#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>

#define MAX_BUFF 32
//장치 변수 
int push_dev;
int dip_dev;
int buzzer_dev;
int fnd_dev;
int text_lcd_dev;
int motor_dev;
int led_dev;
int dot_dev;

unsigned char quit = 0;
// 기기제어를 위한 변수
int motor_action; //모터의 동작,방향, 스피드 
int motor_direction;
int motor_speed;
unsigned char motor_state[3]; // 모터상태제어 
//dot 동작을 위한 배열 
unsigned char fpga_number[10][10] = {
	{ 0x3e,0x7f,0x63,0x73,0x73,0x6f,0x67,0x63,0x7f,0x3e }, // 0
	{ 0x0c,0x1c,0x1c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x1e }, // 1
	{ 0x7e,0x7f,0x03,0x03,0x3f,0x7e,0x60,0x60,0x7f,0x7f }, // 2
	{ 0xfe,0x7f,0x03,0x03,0x7f,0x7f,0x03,0x03,0x7f,0x7e }, // 3
	{ 0x66,0x66,0x66,0x66,0x66,0x66,0x7f,0x7f,0x06,0x06 }, // 4
	{ 0x7f,0x7f,0x60,0x60,0x7e,0x7f,0x03,0x03,0x7f,0x7e }, // 5
	{ 0x60,0x60,0x60,0x60,0x7e,0x7f,0x63,0x63,0x7f,0x3e }, // 6
	{ 0x7f,0x7f,0x63,0x63,0x03,0x03,0x03,0x03,0x03,0x03 }, // 7
	{ 0x3e,0x7f,0x63,0x63,0x7f,0x7f,0x63,0x63,0x7f,0x3e }, // 8
	{ 0x3e,0x7f,0x63,0x63,0x7f,0x3f,0x03,0x03,0x03,0x03 } // 9
};

//
void user_signal1(int sig)
{
	quit = 1;
}

void *t_function(void *data)
{
	pid_t pid;            // process id
	pthread_t tid;        // thread id

	pid = getpid();
	tid = pthread_self();

	char* thread_name = (char*)data;
	int i = 0;

	

	while (1) {
		motor_state[0] = (unsigned char)1;
		motor_state[1] = (unsigned char)0;
		motor_state[2] = (unsigned char)255;
		write(motor_dev, motor_state, 3);
		usleep(1000000);
	}
	close(motor_dev);

}

int main(void) {

	unsigned char push_sw_buff[9]; //푸쉬버튼버퍼
	int buff_size; //푸쉬버튼 버퍼사이즈
	buff_size = sizeof(push_sw_buff);

	// 기기제어를 위한 변수
	unsigned char string[32] = "Welcome to slot machine!push btn";
	unsigned char retval2; // fnd 를 제어하기위한 문자변수
	char verify[4] = { '1','1','1','1' }; //fnd를 사용하기위한 코드
	
	unsigned char state = 0; // 부저상태제어
	unsigned char retval;  // 부저
	unsigned char data; //부저 
	int str_size;
	//** 응용 동작을 위한 변수 **//
	unsigned char data_led[4] = { 0,255,124,64 }; //led  data
	int random_num; // 도트를 위한 난수 
	/*----------------*/ // TEXT_LCD
	int button_clicked = 0; //버튼이 클릭되었는지 확인 

	/*----------------*/ // 쓰레드를 위한 코드 
	pthread_t p_thread[2];
	int thr_id;
	int status;
	char p1[] = "thread_1";


	

	push_dev = open("/dev/fpga_push_switch", O_RDWR);
	buzzer_dev = open("/dev/fpga_buzzer", O_RDWR);
	fnd_dev = open("/dev/fpga_fnd", O_RDWR);
	text_lcd_dev = open("/dev/fpga_text_lcd", O_RDWR);
	motor_dev = open("/dev/fpga_step_motor", O_RDWR);
	led_dev = open("/dev/fpga_led", O_RDWR);
	dot_dev = open("/dev/fpga_dot", O_WRONLY);

	if (dot_dev<0) {
		printf("Device open error : %s\n", "/dev/fpga_dot");
		exit(1);
	}
	if (led_dev<0) {
		printf("Device open error : %s\n", "/dev/fpga_led");
		exit(1);
	}
	if (push_dev<0) {
		printf("Push_Switch Device Open Error\n");
		close(push_dev);
		return -1;
	}
	if (buzzer_dev<0) {
		printf("Buzzer Device Open Error\n");
		close(buzzer_dev);
		return -1;
	}
	if (fnd_dev<0) {
		printf("Fnd Device Open Error\n");
		close(fnd_dev);
		return -1;
	}
	if (text_lcd_dev<0) {
		printf("Text_lcd Device Open Error\n");
		close(text_lcd_dev);
		return -1;
	}
	if (motor_dev<0) {
		printf("Motor Device Open Error\n");
		close(motor_dev);
		return -1;
	}
	
	// init 해주는 과정 

	(void)signal(SIGINT, user_signal1); //초기화 

	//환영메시지 출력 
	write(text_lcd_dev, string, MAX_BUFF);
	//현란한 LED 출력
	for (int i = 0; i < 12; i++) {
		write(led_dev, &data_led[i%4], 1);
		usleep(1000000);
	}

	while(1) { // 계속돌아야되서 무한루프
		int i;
		button_clicked = 0;
		usleep(400000);
		read(push_dev, &push_sw_buff, buff_size);
		

		for (i = 0; i<9; i++) {
			printf("[%d] ", push_sw_buff[i]);
			if (push_sw_buff[i] == 1) {
				button_clicked = 1;
				break;
			}
		}
		
		if (button_clicked == 1)
			break;
		printf("\n");
	}
	// 버튼을 누른것을 인지
	// fnd와 dot값이 움직이는 상황
	
	for(int i=0;i<41;i++){
		random_num = rand() % 10;
		str_size = sizeof(fpga_number[random_num]);
		write(dot_dev, fpga_number[random_num], str_size);
		usleep(50000); //0.05초 
	}

	retval2 = read(fnd_dev, &data, 4);
	if (retval2<0) {
		printf("WriteError!\n");
		return -1;
	}
	//성공시 
	// 모터가 움직이는 상황
	//쓰레드 호출 
	thr_id = pthread_create(&p_thread[0], NULL, t_function, (void *)p1);

	if (thr_id < 0)
	{
		perror("thread create error : ");
		exit(0);
	}
	
	// 

	//write(dot_dev, 1, 10);




	sleep(1);

	

	//환영메시지 출력 
	write(text_lcd_dev, "congraturation!!", MAX_BUFF);


	// led 출력 함수 
	for (int i = 0; i < 120; i++) {
		write(led_dev, &data_led[i % 4], 1);
		usleep(1000000);
	}

	


	close(push_dev);
	close(dip_dev);
	close(buzzer_dev);
	close(fnd_dev);
	close(text_lcd_dev);
	close(motor_dev);
	return 0;

}



