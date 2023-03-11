#include <bits/stdc++.h>

//This program is to accept a user input that gives a time in 24 hour format 
//which is the start of a meeting followed by a comma and then the amount of minutes 
//for the length of the meeting so the input will look like this:
//1340,45    this would be meeting starts at 1340 and lasts for 45 mins
//the program will accept a file that lists all of these lines where the first line
//of the file is the amount of meetings so it would look like this:
//3
//1340,45
//0800,30
//0830,60
//the purpose of this program is find any conflicts of overlapping meetings

using namespace std;

int main()
{
	string temp;
	vector<string> v;
	ifstream meetingFile ("meetings.txt");
	vector<int> startTime;                       //vector to hold the meeting start times in 24 hour format
	vector<int> meetingLength;                   //vector to hold the meeting length in minutes
	vector<int> endTime;                         //vector to hold the end of meeting time
	vector<int> checkingTime;                    //vector to check to make sure the 24 hour format is correct
	int numOfMeetings;                           //int that hold the amount of meetings

	
	
	while (meetingFile >> temp)                  //taking from file and adding to vector
	{
		v.push_back(temp);
	}
	
	//for (int i = 0; i < v.size(); i++)   //Testing if the reading the fild correctly
	//{
	//	cout << v[i] << "\n";
	//}

	numOfMeetings = stoi(v[0]);                  //creating a int with the amount of meetings
	
	for (int i = 0; i < (numOfMeetings+1); i++)    //loop to create vectors of the inputted data
	{
		temp = v[i+1].substr(0, 4);                //gathering the start time from the input file
		int num = stoi(temp);                      //converting the start time to an int
		startTime.push_back(num);                  //adding this int to a vector

		temp = v[i + 1].substr(2, 2);              //gathering last 2 digits of the 24 hour clock to check  
		num = stoi(temp);                          //and make sure it has been added in correctly
		checkingTime.push_back(num);               //converts to int and adds to vector

		temp = v[i + 1].substr(5, 2);              //gathering the meeting length time in minutes
		num = stoi(temp);                          //converting this to an int
		meetingLength.push_back(num);              //adding to a new vector


		//cout << startTime[i] << endl;          //testing vectors of start times
		//cout << meetingLength[i] << endl;      //testing vector of meeting lenth in mins
	}

	for (int i = 0; i < (numOfMeetings+1); i++)       //calculating end times for each meeting
	{

		int num = meetingLength[i] + checkingTime[i];      //adding the meetings length time by the minutes of the meeting start time
		int end;                                           //this is to add the end time up correctly using 60 as a base

		if (num > 120)                                     //if the length and starttime minutes are more than 2 hour, add 200 to the end time plus whatever is left over
		{
			end = 200 + (num-120) + (startTime[i] - checkingTime[i]);  //add 200(2 hours) add whats left (num - 2 hours)
			                                                           //then add this all to the current start time subtracting the minutes becuase we used those in the above equation
			endTime.push_back(end);
		}
		else if (num > 60)                                             //see comment above, just chaging to be 1 hour instead of 2
		{
			end = 100 + (num - 60) + (startTime[i] - checkingTime[i]);
			endTime.push_back(end);
		}
		else                                                          //if we are not going over the hour just add the meeting length to the current start value
		{
			endTime.push_back(meetingLength[i] + startTime[i]);
		}

		if (endTime[i] > 2359)                                    //if the meeting goes past the hour mark just add this new time 
		{
			int rem = meetingLength[i] - (59 - checkingTime[i]);     //finding the remainder of the time past the hour

			if (startTime[i] < 2300)                                //if the start meeting time is under 2300
			{
				endTime[i] = rem - 61;                              //in this case the value will always be somewhere bewteen Midnight and 1 am
			}                                                       //so alwasy have to take an hour out + the reaminder.
			else if (rem > 60)                                      
			{
				endTime[i] = 100 + (rem - 60) - 1;                 //if the hour is past 2300 and the remainder is more than 60 mins
			}                                                      //then the time will fall somewhere between 1-2am, so set the hour to 100 and then use the reaminder minus 1 hour
			else
			{
				endTime[i] = rem-1;                               //if the remainder is under 60 and the start hour is 2300+, then the end time is just the remainder 
			}
			
		}
		//cout << endTime[i] << endl;
	}

	for (int i = 0; i < (numOfMeetings+1); i++)           //error checking
	{
		if (meetingLength[i] > 90)                   //if the meeting is longer than 90 mins
		{
			cout << "Meeting Length is greater than 90 minutes. Please re-enter data\n";
			return 0;
		}

		if (startTime[i] > 2400)              //if the start time is not under 2400 hours
		{
			cout << "Start time is greater than 2400. Please re-enter data\n";
			return 0;
		}

		if (checkingTime[i] > 60)                //if the start time was input incorrectly more than 60 mins per hour
		{
			cout << "Meeting start time has been entered incorrectly. Please check your data\n";
			return 0;
		}
	}

	//the below code is going to compare the start times to end times to see if there is any overlap. I am skipping chcking if 
	//a new day is part of the specturm as the code above does check for past midnight but it does not say it is a new day
	//so ill just assume it is the same time and not a new day for now

	int k = 1;

	for (int i = 0; i < (numOfMeetings-1); i++)       //loop runs through every line but the last checking for any meeting overlaps
	{                                                 //the loop will check if the start time of the first meeting is bigger, smaller or the same as the next start time
		for (int j = k; j < (numOfMeetings+1); j++)   //if the first meeting start time is bigger, itll check if the it is smaller than the second meetsin end time 
		{                                             //if it is then there is an overlap. If the first start meeting is smaller, then itll check
			if (startTime[i] > startTime[j] && startTime[i] < endTime[j])    //if the ending time of the first meeting is bigger than the start of the second
			{                                                                //if it is, then there is an overlap
				cout << "overlap\n";                                         //the last check is simply if two meetings start at the same time
			}                                                                //they will definitly overlap
			if (startTime[i] < startTime[j] && endTime[i] > startTime[j])
			{
				cout << "overlap\n";
			}
			if (startTime[i] == startTime[j])
			{
				cout << "overlap\n";
			}
		}

		k++;                                               //i used value k to set the second loop so that I was not resetting the value of j each iteration
	}                                                      //as i did not need to search values twice, which is what would happen if j kept restting to a lower value

	 
	


 }