#include <iostream>
#include <string>
#include "frc/WPILib.h"
#include "ctre/Phoenix.h"
#include "frc/SerialPort.h"

using namespace frc;

class Robot: public TimedRobot {
public:
	WPI_TalonSRX * _rghtFront = new WPI_TalonSRX(4);
	WPI_TalonSRX * _rghtFollower = new WPI_TalonSRX(2);
	WPI_TalonSRX * _leftFront = new WPI_TalonSRX(3);
	WPI_TalonSRX * _leftFollower = new WPI_TalonSRX(1);
	frc::Spark meca{0};
	frc::Spark motor{1};
	frc::VictorSP recogedor{2};

	DifferentialDrive * _diffDrive = new DifferentialDrive(*_leftFront,
			*_rghtFront);

	Joystick * _joystick = new Joystick(0);
	Joystick * rojo = new Joystick(1);


	DigitalOutput * led_rojo = new DigitalOutput(2);
	DigitalOutput * led_verde = new DigitalOutput(3);
	DigitalOutput * led_azul = new DigitalOutput(4);
	DigitalInput * contacto = new DigitalInput(5);

	int contador = 0;
	int numeros = 0;
	int inicio = 0;
	double derecho;
	Faults _faults_L;
	Faults _faults_R;

	void TeleopPeriodic() {

		std::stringstream work;
		derecho = 0;
		/* Single motors */
		meca.EnableDeadbandElimination(true);
		motor.EnableDeadbandElimination(true);
		recogedor.EnableDeadbandElimination(true);

		/* get gamepad stick values */
		double forw = -1 * _joystick->GetRawAxis(1); /* positive is forward */
		double turn = +1 * _joystick->GetRawAxis(4); /* positive is right */
		bool boton = rojo->GetRawButton(6);
		bool buton = rojo->GetRawButton(5);
		bool be = rojo->GetRawButton(3);
		bool color = _joystick->GetRawButton(5);
		bool gatiyo = _joystick->GetRawButton(6);
		bool zelda = rojo->GetRawButton(8);
		SmartDashboard::PutNumber("Boton Piso1",boton);
		SmartDashboard::PutNumber("Boton Piso2 ",buton);
		if ((boton == 1)and(contador<10)){
			contador++;
			meca.Set(-1);
			motor.Set(-1);
			led_rojo->Set(1);
			led_verde->Set(0);
			led_azul->Set(0);
		}
		if(contador==10){
			meca.Set(0);
			motor.Set(0);
			led_rojo->Set(1);
			led_verde->Set(0);
			led_azul->Set(0);
		}

		if ((buton == 1)and(numeros<200)){
			numeros++;
			meca.Set(-1);
			motor.Set(-1);
			led_rojo->Set(1);
			led_verde->Set(0);
			led_azul->Set(0);
		}
		if(numeros==200){
			meca.Set(0);
			motor.Set(0);
			led_rojo->Set(0);
			led_verde->Set(1);
			led_azul->Set(0);
		}

		if((zelda == 1)and(inicio<400)){
			inicio++;
			meca.Set(-1);
			motor.Set(-1);
			led_rojo->Set(0);
			led_verde->Set(0);
			led_azul->Set(1);
		}
		if(inicio==400){
			meca.Set(0);
			motor.Set(0);
			led_rojo->Set(0);
			led_verde->Set(0);
			led_azul->Set(1);

		}


		if((color==1)and(gatiyo==0)){
			recogedor.Set(-1);
		}else if((color==0)and(gatiyo==1)){
			recogedor.Set(1);
		}

/*---------------------------------------------------------------------*/
		if((be == 1)){
			meca.Set(0.15);
			motor.Set(0.15);
			led_rojo->Set(1);
			led_verde->Set(0);
			led_azul->Set(1);
		}
/*=====================================*/
		if((boton == 0)and(buton==0)and(be==0)and(color==0)and(gatiyo==0)and(zelda==0)){
			 contador = 0;
			 numeros = 0;
			 inicio = 0;
			 meca.Set(0);
			 motor.Set(0);
			 recogedor.Set(0);
			 led_rojo->Set(1);
			 led_azul->Set(1);
			 led_verde->Set(1);
		}

		SmartDashboard::PutNumber("Piso 1: ",contador);
		SmartDashboard::PutNumber("Piso 2: ",numeros);

		SmartDashboard::PutNumber("Boton Regreso ",be);
		SmartDashboard::PutNumber("color ",color);
		SmartDashboard::PutNumber("gatiyo ",gatiyo);

		/* deadband gamepad 10%*/
		if (fabs(forw) < 0.10)
			forw = 0;
		if (fabs(turn) < 0.10)
			turn = 0;
		/* drive robot */
		if((forw>=-0.65)and(forw<=0.65)){
			derecho=forw;
		}
		if(forw>0.65){
			derecho=0.6;
		}
		if(forw<-0.65){
			derecho=-0.65;
		}
		_diffDrive->ArcadeDrive(derecho, turn, false);

	}
	void AutonomousPeriodic() {
		while(true){
			TeleopPeriodic();
		}
	}

	void RobotInit() {

		/* Camera server */
		CameraServer::GetInstance()->StartAutomaticCapture(0);

		/* factory default values */
		_rghtFront->ConfigFactoryDefault();
		_rghtFollower->ConfigFactoryDefault();
		_leftFront->ConfigFactoryDefault();
		_leftFollower->ConfigFactoryDefault();

		/* set up followers */
		_rghtFollower->Follow(*_rghtFront);
		_leftFollower->Follow(*_leftFront);

		/* [3] flip values so robot moves forward when stick-forward/LEDs-green */
		_rghtFront->SetInverted(false);
		_rghtFollower->SetInverted(false);
		_leftFront->SetInverted(false);
		_leftFollower->SetInverted(false);

		/* [4] adjust sensor phase so sensor moves
		 * positive when Talon LEDs are green */
		_rghtFront->SetSensorPhase(true);
		_leftFront->SetSensorPhase(true);

		/*
		* WPI drivetrain classes defaultly assume left and right are opposite. call
		* this so we can apply + to both sides when moving forward. DO NOT CHANGE
		*/
		_diffDrive->SetRightSideInverted(true);

	}

private:
};

START_ROBOT_CLASS(Robot)
