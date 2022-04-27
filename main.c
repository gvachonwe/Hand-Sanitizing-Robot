// Function Prototypes
void configureAllSensors();
int getGroupMembers();
int getLoops();
void driveForward();
void dispenseSanitizer(int groupMembers);
void playChime();
void driveBackward();


// Global Constants
const int COUNTS = 360;
const float RADIUS = 2.75;
const int WAIT = 10000;
const int DRIVE = 40;
const int ROTATION = 170;
const int DISTANCE = DRIVE*COUNTS/(2*PI*RADIUS);


// Emergency Stop
task killSwitch()
{
    while(!getButtonPress(buttonEnter))
    {}

    stopAllTasks();
}


// Main Program
task main()
{
    configureAllSensors();

    int numMembers = getGroupMembers();
    wait1Msec(500);

    int numLoops = getLoops();
    wait1Msec(500);

    startTask(killSwitch);

    for(int loopCount = 0; loopCount < numLoops; loopCount++)
    {
        clearTimer(T1);

        while (time1[T1] < WAIT)
        {}

        displayString(8, "Time to sanitize!");
        wait1Msec(1000);

        driveForward();

        dispenseSanitizer(numMembers);

        playChime();

        driveBackward();

        eraseDisplay();
    }

    stopTask(killSwitch);
}


// Functions
void configureAllSensors()
{
    SensorType[S4] = sensorEV3_Ultrasonic;

    SensorType[S1] = sensorEV3_Gyro;
    wait1Msec(50);
    SensorMode[S1] = modeEV3Gyro_Calibration;
    wait1Msec(50);
    SensorMode[S1]= modeEV3Gyro_RateAndAngle;

    nMotorEncoder[motorA] = 0;
}


int getGroupMembers()
{
    int groupDisplay = 0;

    displayString(7, "Press UP to add members and");
    displayString(8, "DOWN to remove members.");
    displayString(9, "Press ENTER to start.");

    while(!getButtonPress(buttonAny))
    {}

    int enterPressG = 0;

    do
    {
        int pressRepG = 0;

        if(getButtonPress(buttonUp))
        {
            eraseDisplay();

            pressRepG = 1;
        }
        else if(getButtonPress(buttonDown))
        {
            eraseDisplay();

            pressRepG = -1;
        }
        else if(getButtonPress(buttonEnter))
        {
            enterPressG = 1;
        }

        if(pressRepG == 1 || pressRepG == -1)
        {
            groupDisplay = groupDisplay + pressRepG;

            if(groupDisplay > 0)
            {
                displayCenteredBigTextLine(8, "%d", groupDisplay);
                wait1Msec(500);
            }
            else
            {
                displayCenteredBigTextLine(8, "1");
                wait1Msec(500);
            }
        }
    } while(enterPressG == 0);

    eraseDisplay();

    return groupDisplay;
}


int getLoops()
{
    int loopDisplay = 0;

    displayString(7, "Press UP to add time and");
    displayString(8, "DOWN to remove time.");
    displayString(9, "Press ENTER to start.");

    while (!getButtonPress(buttonAny))
    {}

    int enterPressL = 0;

    do
    {
        int pressRepL = 0;

        if(getButtonPress(buttonUp))
        {
            eraseDisplay();

            pressRepL = 1;
        }
        else if(getButtonPress(buttonDown))
        {
            eraseDisplay();

            pressRepL = -1;
        }
        else if(getButtonPress(buttonEnter))
        {
            enterPressL = 1;
        }

        if(pressRepL == 1 || pressRepL == -1)
        {
            loopDisplay = (loopDisplay + pressRepL);

            if (loopDisplay > 0)
            {
                displayCenteredBigTextLine(8, "%d mins", loopDisplay*30);
                wait1Msec(500);
            }
            else
            {
                displayCenteredBigTextLine(8, "30 mins");
                wait1Msec(500);
            }
        }
    } while(enterPressL == 0);

    eraseDisplay ();

    return loopDisplay;
}


void driveForward()
{
    configureAllSensors();

    motor[motorA] = motor[motorD] = 25;

    const int PATH = 20;

    while(DISTANCE > nMotorEncoder[motorA] && SensorValue[S4] > PATH)
    {}

    motor[motorA] = motor[motorD] = 0;
}


void dispenseSanitizer(int groupMembers)
{
    const int HAND = 15;

    for(int count = 0; count < groupMembers; count++)
    {
        while(SensorValue[S4] > HAND)
        {}

        motor[motorC] = -100;
        wait1Msec(1000);

        motor[motorC] = 50;
        wait1Msec(100);

        motor[motorC] = 0;
        wait1Msec(500);

    }
    eraseDisplay();
}


void playChime ()
{
    setSoundVolume(20);

    playSoundFile("Magic wand.rsf");
    wait1Msec(2000);

    setSoundVolume(50);

    playSoundFile("Good job.rsf");
    wait1Msec(1000);
}


void driveBackward()
{
    configureAllSensors();

    motor[motorA] = -10;
    motor[motorD] = 10;

    while(getGyroDegrees(S1) < ROTATION)
    {}

    driveForward();

    configureAllSensors();

    motor[motorA] = -10;
    motor[motorD] = 10;

    while(getGyroDegrees(S1) < ROTATION)
    {}

    motor[motorA] = motor[motorD] = 0;
}