/*
 * Test.c
 *
 *  Created on: Oct 16, 2019
 *      Author: Marktronix
 */
#define F_CPU 16000000UL
#include <util/delay.h>
#include <stdlib.h>
#include "../utils/custom_types.h"
#include "../hal/lcd/lcd.h"
#include "../mcal/DIO/Dio.h"
#include"../hal/keypad/keypad.h"
#include "../mcal/usart/usart.h"
#include "../mcal/timer1_pwm/timer1_pwm.h"
#define first_address_of_user_in_hex 0x60
#define last_address_of_user_in_hex 0x99

#define first_address_of_user 60
#define last_address_of_user 99

#define first_address_of_admin_in_hex 0x20
#define last_address_of_admin_in_hex 0x59

#define first_address_of_admin 20
#define last_address_of_admin 59

#define first_address_of_superadmin 0x0
#define last_address_of_superadmin 0x16

//#define first_address_of_superadmin2 0x08

#define last_index_user 0x200
#define last_index_admin 0x300
#define last_index_superadmin 0x350

void user();
void admin();
void addpesron(u8_t thelastadress ,u8_t name[]);
u8_t check_id1(u8_t first_address_eeprom,u8_t secound_address_eeprom);
void check_person(u8_t checkid3);
void addressesofeeprom();
void motor_run();
void super_mood();

void solution_challenge(void)
{
	init_usart(9600);
	init_keypad();
	init_lcd();
	dio_channel_config(Dio_channel_c3,output);
	dio_channel_config(Dio_channel_c4,output);

	//if(EEPROM_read(last_index_superadmin)!=0x08)
	/*here you should build the code the upload the code the make a comment in the next line */
	//addressesofeeprom();
	/*after you make a comment make build again and upload the code again thanke*/
	lcd_write_text("enter password  ",1,1);
	while(1)
	{
		user();
	}
}
void user()
{
	u8_t checkid,checkpass;
	checkid=check_id1(first_address_of_user,last_address_of_user);
	check_person(checkid);
}
void admin()
{
	u8_t checked_id,checked_password,answer,answer2;
	u16_t last_address=0;
	lcd_clear();
	lcd_write_text("1-open",1,1);
	lcd_write_text("2-add user",2,1);
	answer=get_key_clicked();
	if(answer=='1')
	{
		motor_run();
	}else if(answer=='2')
	{
		last_address=EEPROM_read(last_index_user);
		addpesron(last_address,"user");
	}
}
void addpesron(u8_t thelastadress ,u8_t name[])
{


	u8_t new_id[4],i=0;
	u8_t new_password[4],z=0;
	lcd_clear();
	//usart_write_text("\n\r add the new id of");
	//usart_write_text(name);
	lcd_write_text("enter new password",1,1);
	new_id[0]=get_key_clicked();
	lcd_write_char('*',2,1);

	while(new_id[i]!='D')
	{
		new_id[++i]=get_key_clicked();
		lcd_write_char_cp('*');

	}
	for(i=thelastadress;i<thelastadress+4;i++)
	{
		EEPROM_write(i,new_id[z]);
		z++;
	}
	if(name[0]=='u')
	{
		EEPROM_write(last_index_user,thelastadress+4);
	}
}
void addressesofeeprom()
{
	u8_t x=0,i=0;
	u8_t superadmin_id_and_pass[8]={'1','5','5','4','5','6','8','8'};

	for( i=0;i<16;i++)
	{
		EEPROM_write(i,superadmin_id_and_pass[i]);
	}
	for(i=0x60;i<0x68;i++)
	{
		EEPROM_write(i,superadmin_id_and_pass[x]);
		x++;
	}
	EEPROM_write(last_index_admin,first_address_of_admin_in_hex);
	EEPROM_write(last_index_user,0x68);
	EEPROM_write(last_index_superadmin,0x08);
}

u8_t check_id1(u8_t first_address_eeprom,u8_t secound_address_eeprom)
{
	u8_t login_id[4],x=first_address_eeprom,z=0,i=0,counter=0,admin[]={'1','5','5','4'},check_the_admin=0;
	u8_t text[10];

	if(first_address_eeprom == first_address_of_user)
	{
		x=first_address_of_user_in_hex;

	}else if(first_address_eeprom == first_address_of_admin)
	{
		x=first_address_of_admin_in_hex;

	}

	//usart_write_text("\n\r enter your password ");
	lcd_clear();
	lcd_write_text("enter password",1,1);
	login_id[i]=get_key_clicked();
	lcd_write_char('*',2,1);
	while(login_id[i]!='D')
	{
		login_id[++i]=get_key_clicked();
		lcd_write_char_cp('*');
	}
	for(i=first_address_eeprom;i<=secound_address_eeprom;i++)
	{
		if(i%5)
		{

			if(EEPROM_read(x)==login_id[z])
			{
				counter=counter+1;
				if(counter==4)
				{

					for(i=0;i<4;i++)
					{
						if(admin[i]==login_id[i])
						{
							check_the_admin=check_the_admin+1;
						}else
						{
							check_the_admin=check_the_admin-1;
						}
					}
					break;
				}else
				{

			    }
				}else
				{
					counter=counter-1;
				}
				z++;
				x++;
		}else
		{
			z=0;
			counter=0;
		}
	}
	if(counter == 4 && check_the_admin ==2)
	{
		counter=4;//admin
	}else if(counter == 4 && check_the_admin !=4)
	{
		counter=20;//user
	}
	return counter;
}
void check_person(u8_t checkid3)
{
	static counter=0;
	u8_t reslut=0;
	if(checkid3 ==4)
	{
		//usart_write_text("\n\r welcom");
		admin();

	}else if(checkid3 ==20  )
	{
			motor_run();
	}else if(counter!=6)
	{
		counter=counter+1;
		user();
	}else if(counter==6)
	{
		counter=0;
		lcd_clear();
		lcd_write_text("in super mood",1,1);
		usart_write_text("machine locked");
		_delay_ms(1000);
		super_mood();
		admin();
	}
}
void motor_run()
{
	lcd_clear();
	lcd_write_text("welcom",1,1);
	dio_channel_set(Dio_channel_c3,high);
	dio_channel_set(Dio_channel_c4,low);
	_delay_ms(1000);
	dio_channel_set(Dio_channel_c3,low);
	dio_channel_set(Dio_channel_c4,low);
}
void super_mood()
{

	u8_t login_id[4],i=0,check_the_admin,admin[]={'1','5','5','4'};
	hi :
	lcd_clear();
	lcd_write_text("enter password",1,1);
	login_id[i]=get_key_clicked();
	lcd_write_char('*',2,1);
	while(login_id[i]!='D')
	{
		login_id[++i]=get_key_clicked();
		lcd_write_char_cp('*');
	}
	for(i=0;i<4;i++)
	{
		if(admin[i]==login_id[i])
		{
			check_the_admin=check_the_admin+1;
		}else
		{
			check_the_admin=check_the_admin-1;
		}
	}
	if(check_the_admin!=2)
	{
		i=0;
		check_the_admin=0;
		goto hi;
	}

}

