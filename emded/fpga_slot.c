#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <string.h>

#define MAX_BUFF 32

/*
void user_signal1(int sig)
{
	quit = 1;
}
*/

int main(void) {

	unsigned char push_sw_buff[9]; //푸쉬버튼버퍼
	int buff_size; //푸쉬버튼 버퍼사이즈
	buff_size = sizeof(push_sw_buff);
	
	int push_dev;
	int dip_dev;
	int buzzer_dev;
	int fnd_dev;
	int text_lcd_dev;
	int motor_dev;
	int led_dev;
	int dot_dev;
	// 기기제어를 위한 변수
	unsigned char string[32] = "Welcome to slot machine!push btn";
	unsigned char retval2; // fnd 를 제어하기위한 문자변수
	char verify[4] = { '1','1','1','1' }; //fnd를 사용하기위한 코드
	int motor_action; //모터의 동작,방향, 스피드 
	int motor_direction;
	int motor_speed;
	unsigned char motor_state[3]; // 모터상태제어 
	//** 응용 동작을 위한 변수 **//
	unsigned char data_led[4] = { 0,255,124,64 }; //led  data

	/*----------------*/ // TEXT_LCD
	int button_clicked = 0; //버튼이 클릭되었는지 확인 

	/*----------------*/ // LED
	
	void Clear_LED();
	unsigned char CalcLED();


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


	//(void)signal(SIGINT, user_signal1);
	//환영메시지 출력 
	write(text_lcd_dev, string, MAX_BUFF);
	//현란한 LED 출력
	for (int i = 0; i < 12; i++) {
		write(led_dev, &data_led[i%4], 1);
		usleep(1000000);
	}
	while (1) { // 계속돌아야되서 무한루프
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

	
	//write(dot_dev, 1, 10);

	retval2 = write(fnd_dev, verify, 4);
	if (retval2<0) {
		printf("WriteError!\n");
		return -1;
	}
	sleep(1);

	motor_state[0] = (unsigned char)1;
	motor_state[1] = (unsigned char)1;
	motor_state[2] = (unsigned char)255;
	write(motor_dev, motor_state, 3);

	for (int i = 0; i < 12; i++) {
		write(led_dev, &data_led[i % 4], 1);
		usleep(1000000);
	}

	//환영메시지 출력 
	write(text_lcd_dev, "sex! sex on  beach ,y da", MAX_BUFF);
	close(push_dev);
	close(dip_dev);
	close(buzzer_dev);
	close(fnd_dev);
	close(text_lcd_dev);
	close(motor_dev);
	return 0;

}

