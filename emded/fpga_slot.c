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
unsigned char state = 0; // 부저상태제어
unsigned char retval;  // 부저
unsigned char data1 = 1; //부저 
int buzzer_state = 0;
// 응용동작을 위한 함수
unsigned char current_data[8] = { 1,2,4,8,16,32,64,128 };
unsigned char start_data[4] = { 17,34,68,136 };
unsigned char load_data[4] = { 24,36,66,129 };
unsigned char exit_data[4] = { 204, 102, 51, 153 };


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

void user_signal1(int sig)
{
	quit = 1;
}

void *t_function(void *data);

void *t_function2(void *data);

void *t_function3(void *data);

void *t_function4(void *data);
void open_devices(void);
void close_devices(void);
int main(void) {
	srand(time(NULL)); // 랜점 난수의 시드값변경

	// 기기제어를 위한 변수

	unsigned char string[32] = "Welcome to slot machine!push btn";
	unsigned char retval2; // fnd 를 제어하기위한 문자변수
	unsigned char verify[4]; //fnd를 사용하기위한 코드
							 //unsigned char data[4]; // fnd  제어를 위한 
	unsigned char push_sw_buff[9]; //푸쉬버튼버퍼
	int buff_size; //푸쉬버튼 버퍼사이즈
	buff_size = sizeof(push_sw_buff); //푸쉬버튼 사이즈 할당
	int str_size; 

	//*응용 동작을 위한 변수 **//

	int random_num; // 도트를 위한 난수 
	int random_num2; // 도트를 위한 난수 

	int button_clicked = 0; //버튼이 클릭되었는지 확인 

	/*쓰레드를 위한 코드 */ 

	pthread_t p_thread[4];
	int thr_id;
	int status;
	char p1[] = "thread_1";
	char p2[] = "thread_2";
	char p3[] = "thread_3";
	char p4[] = "thread_4";
	int trash;
	open_devices();
	while (1) { // 실패시 계속 무한루프
		// init 해주는 과정 
		char* text_value = "0000";
		retval2 = write(fnd_dev, text_value, 4);
		//부저 초기화
		data1 = 1;
		(void)signal(SIGINT, user_signal1); // 초기화 
		retval = write(buzzer_dev, &data1, 1);
		//모터 초기화
		motor_state[0] = (unsigned char)0;
		motor_state[1] = (unsigned char)0;
		motor_state[2] = (unsigned char)255;
		write(motor_dev, motor_state, 3);
		//환영메시지 출력 
		write(text_lcd_dev, "                                ", MAX_BUFF);
		write(text_lcd_dev, "Welcome to slot machine!push btn", MAX_BUFF);

		//현란한 LED 출력
		thr_id = pthread_create(&p_thread[1], NULL, t_function2, (void *)p2);

		// fnd 출력
		
		for (int i = 0; i < str_size; i++)
		{
			verify[i] = text_value[i] - 0x30;
		}
		
		//버튼 클릭전 
		while (1) { 
			int i;
			button_clicked = 0;
			usleep(400000);
			read(push_dev, &push_sw_buff, buff_size);
			
			for (i = 0; i < 9; i++) {
				//	printf("[%d] ", push_sw_buff[i]);
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
		//버튼을 누름과 동시에  모터 돌린다. 
		motor_state[0] = (unsigned char)1;
		motor_state[1] = (unsigned char)0;
		motor_state[2] = (unsigned char)255;
		write(motor_dev, motor_state, 3);

		for (int i = 0; i < 41; i++) {
			random_num = rand() % 10;
			str_size = sizeof(fpga_number[random_num]);
			write(dot_dev, fpga_number[random_num], str_size);
			usleep(50000); //0.05초 
		}
		text_value = "0007";
		for (int i = 0; i < str_size; i++)
		{
			verify[i] = text_value[i] - 0x30;
		}
		write(dot_dev, fpga_number[7], str_size);
		retval2 = write(fnd_dev, &verify, 4);
		usleep(1000000); //1초 

		for (int i = 0; i < 41; i++) {
			random_num = rand() % 10;
			str_size = sizeof(fpga_number[random_num]);
			write(dot_dev, fpga_number[random_num], str_size);
			usleep(50000); //0.05초 
		}
		text_value = "0077";
		for (int i = 0; i < str_size; i++)
		{
			verify[i] = text_value[i] - 0x30;
		}
		write(dot_dev, fpga_number[7], str_size);
		retval2 = write(fnd_dev, &verify, 4);
		usleep(1000000); //1초 

		for (int i = 0; i < 41; i++) {
			random_num = rand() % 10;
			str_size = sizeof(fpga_number[random_num]);
			write(dot_dev, fpga_number[random_num], str_size);
			usleep(50000); //0.05초 
		}
		text_value = "0777";
		for (int i = 0; i < str_size; i++)
		{
			verify[i] = text_value[i] - 0x30;
		}
		write(dot_dev, fpga_number[7], str_size);
		retval2 = write(fnd_dev, &verify, 4);
		usleep(1000000); //1초 

		random_num2 = rand() % 20 + 41;
		for (int i = 0; i < random_num2; i++) {
			random_num = rand() % 10;
			str_size = sizeof(fpga_number[random_num]);
			write(dot_dev, fpga_number[random_num], str_size);
			usleep(50000); //0.05초 
		}
		random_num = rand() % 5 + 3;
		write(dot_dev, fpga_number[random_num], str_size);
		text_value = "0777";
		char adsa[4] = { '0' + random_num,'7','7','7' };
		text_value = adsa;
		for (int i = 0; i < str_size; i++)
		{
			verify[i] = text_value[i] - 0x30;
		}
		
		retval2 = write(fnd_dev, &verify, 4);
		usleep(1000000); //1초 
		if (adsa[0] == '7') {
			//성공시 
			// 모터가 움직이는 상황
			thr_id = pthread_create(&p_thread[0], NULL, t_function, (void *)p1);
			thr_id = pthread_create(&p_thread[2], NULL, t_function3, (void *)p3);
			if (thr_id < 0)
			{
				perror("thread create error : ");
				exit(0);
			}

			sleep(1);

			write(text_lcd_dev, "                                ", MAX_BUFF);
			
			//환영메시지 출력 
			write(text_lcd_dev, "congraturation!!                ", MAX_BUFF);
			buzzer_state = 1;
			thr_id = pthread_create(&p_thread[3], NULL, t_function4, (void *)p4);
			usleep(100000000); //100초 
		}
		else {
			//실패메시지 출력 
			buzzer_state = 0;
			thr_id = pthread_create(&p_thread[3], NULL, t_function4, (void *)p4);
			write(text_lcd_dev, "                                ", MAX_BUFF);
			
			write(text_lcd_dev, "you lose!       Try again       ", MAX_BUFF);
		}

		pthread_join(p_thread[0], (void **)&trash);
		pthread_join(p_thread[1], (void **)&trash);
		pthread_join(p_thread[2], (void **)&trash);

		usleep(4000000); //1초 
	}
	close_devices();
	return 0;

}


void *t_function(void *data)
{
	pid_t pid;            // process id
	pthread_t tid;        // thread id

	pid = getpid();
	tid = pthread_self();

	char* thread_name = (char*)data;
	int i = 0;



	for (i = 0; i < 100; i++) {
		motor_state[0] = (unsigned char)1;
		motor_state[1] = (unsigned char)i % 2;
		motor_state[2] = (unsigned char)255;
		write(motor_dev, motor_state, 3);
		usleep(200000);
	}
	//close(motor_dev);
	//return;
}

void *t_function2(void *data)
{
	pid_t pid;            // process id
	pthread_t tid;        // thread id

	pid = getpid();
	tid = pthread_self();
	unsigned char init_led = 0;

	for (int i = 0; i < 4; i++) {

		write(led_dev, &start_data[i], 1);

		usleep(100000);

	}
	for (int j = 3; j >= 0; j--) {

		write(led_dev, &start_data[j], 1);

		usleep(100000);

	}
	for (int i = 0; i < 4; i++) {

		write(led_dev, &start_data[i], 1);

		usleep(100000);

	}
	for (int j = 3; j >= 0; j--) {

		write(led_dev, &start_data[j], 1);

		usleep(100000);

	}


	for (int i = 0; i < 4; i++) {

		write(led_dev, &start_data[i], 1);

		usleep(100000);

	}
	for (int j = 3; j >= 0; j--) {

		write(led_dev, &start_data[j], 1);

		usleep(100000);

	}
	for (int i = 0; i < 4; i++) {

		write(led_dev, &start_data[i], 1);

		usleep(100000);

	}
	for (int j = 3; j >= 0; j--) {

		write(led_dev, &start_data[j], 1);

		usleep(100000);

	}
	write(led_dev, &init_led, 1);

}

void *t_function3(void *data) {

	pid_t pid;            // process id
	pthread_t tid;        // thread id

	pid = getpid();
	tid = pthread_self();
	int i;
	for (i = 0; i < 10; i++) {
		for (int i = 0; i < 4; i++) {

			write(led_dev, &start_data[i], 1);

			usleep(100000);

		}
		for (int j = 3; j >= 0; j--) {

			write(led_dev, &start_data[j], 1);

			usleep(100000);

		}
		for (int i = 0; i < 4; i++) {

			write(led_dev, &load_data[i], 1);

			usleep(100000);

		}

		for (int j = 3; j >= 0; j--) {

			write(led_dev, &load_data[j], 1);

			usleep(100000);

		}
		for (int i = 0; i < 4; i++) {

			write(led_dev, &exit_data[i], 1);

			usleep(100000);

		}

		for (int j = 4; j < 0; j--) {

			write(led_dev, &exit_data[j], 1);

			usleep(100000);

		}
	}
	write(led_dev, 0, 1);
}



void *t_function4(void *data) {

	pid_t pid;            // process id
	pthread_t tid;        // thread id

	pid = getpid();
	tid = pthread_self();

	int i;
	if (buzzer_state == 1) {
		if (buzzer_state == 0) {
			for (i = 0; i <100; i++)
			{
				if (i % 2 == 1)
				{
					data1 = 1;
				}
				else
				{
					data1 = 0;
				}
				retval = write(buzzer_dev, &data1, 1);
				usleep(100000);
			}
		}
		else {
			for (int i = 0; i < 100; i++)
			{
				if (i % 2 == 1)
				{
					data1 = 1;
				}
				else
				{
					data1 = 0;
				}
				retval = write(buzzer_dev, &data1, 1);
				usleep(500000);
			}

		}
	}

	else {
		if (buzzer_state == 0) {
			for (i = 0; i <8; i++)
			{
				if (i % 2 == 1)
				{
					data1 = 1;
				}
				else
				{
					data1 = 0;
				}
				retval = write(buzzer_dev, &data1, 1);
				usleep(100000);
			}
		}
		else {
			for (int i = 0; i < 8; i++)
			{
				if (i % 2 == 1)
				{
					data1 = 1;
				}
				else
				{
					data1 = 0;
				}
				retval = write(buzzer_dev, &data1, 1);
				usleep(500000);
			}

		}
	}

}

void open_devices(void) {

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
		
	}
	if (buzzer_dev<0) {
		printf("Buzzer Device Open Error\n");
		close(buzzer_dev);
		
	}
	if (fnd_dev<0) {
		printf("Fnd Device Open Error\n");
		close(fnd_dev);
		
	}
	if (text_lcd_dev<0) {
		printf("Text_lcd Device Open Error\n");
		close(text_lcd_dev);
		
	}
	if (motor_dev<0) {
		printf("Motor Device Open Error\n");
		close(motor_dev);
		
	}

}
void close_devices(void) {
	close(push_dev);
	close(dip_dev);
	close(buzzer_dev);
	close(fnd_dev);
	close(text_lcd_dev);
	close(motor_dev);
}