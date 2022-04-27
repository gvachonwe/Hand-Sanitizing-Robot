
task main()
{
	int currentDisplay = 0;

	displayString(7, "Use Up to add members and");
	displayString(8, "Down to delete members.");
	displayString(9, "Press Enter to start.");

	while (!getButtonPress(buttonAny))
	{}

	eraseDisplay();

	do
	{
		int press = 0;

		if (getButtonPress(buttonUp))
		{
			press = 1;
		}
		else if (getButtonPress(buttonDown))
		{
			press = -1;
		}

		currentDisplay = currentDisplay + press;

		displayCenteredBigTextLine(8, "%d", currentDisplay);
		wait1Msec(500);

	} while (!getButtonPress(buttonEnter));

	eraseDisplay ();

}
