
task main()
{
	motor[motorC] = -100;
	wait1Msec(2000);

	motor[motorC] = 70;
	wait1Msec(100);
}
