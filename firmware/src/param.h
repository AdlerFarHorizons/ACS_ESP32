
File myFile;
char* param_file = "/param.txt";

void parseAndSave(char *buff);

void readparam(fs::FS &fs)
{
  // if file is not present, write standard parameters to SD
  if (!SD.exists(param_file)) {
    myFile = fs.open(param_file, FILE_WRITE);                           // open the file.

    if (myFile) {
      Serial.print("Writing Standard Parameters to param.txt ...");    // if the file opened okay, write to it:

      myFile.print("FLIGHT_NUMBER = "); myFile.println(flightNum);
      myFile.print("RELEASE_POS = "); myFile.println(RELEASE_POS);
      myFile.print("OPEN_POS = "); myFile.println(OPEN_POS);
      myFile.print("CLOSED_POS = "); myFile.println(CLOSED_POS);
      myFile.print("VENT_ALT = "); myFile.println(VENT_ALT);
      myFile.print("FLOAT_TIME = "); myFile.println(FLOAT_TIME);
      myFile.print("MAXFLOATALT = "); myFile.println(MAXFLOATALT);

      myFile.close();  Serial.println("done.");                       // close the file:
    }
  }
  else {
    Serial.println("Parameter File already exists.");                      // if the file didn't open, print an error:
  }

  // open the file for reading:
  myFile = fs.open(param_file);
  if (myFile) {
    Serial.println("\n***************************** \n");
    Serial.print("Filename: "); Serial.println(param_file );

    // read from the file
    char buffer[40]; // May need to be a bit bigger if you have long names
    byte index = 0;
    while (myFile.available())
    {
      char c = myFile.read();
      if (c == '\n' || c == '\r') // Test for <cr> and <lf>
      {
        parseAndSave(buffer);
        index = 0;
        buffer[index] = '\0'; // Keep buffer NULL terminated
      }
      else
      {
        buffer[index++] = c;
        buffer[index] = '\0'; // Keep buffer NULL terminated
      }
    }

    myFile.close();                                                 // close the file:
  } else {
    Serial.println("error opening test.txt");                 // if the file didn't open, print an error
  }
  Serial.print("Flight Number: ");   Serial.println(flightNum);
  Serial.print("Release Position: ");   Serial.println(RELEASE_POS);
  Serial.print("Open Position: ");      Serial.println(OPEN_POS);
  Serial.print("Closed Position: ");    Serial.println(CLOSED_POS);
  Serial.print("Venting Altitude: ");   Serial.println(VENT_ALT);
  Serial.print("Float Time: ");         Serial.println(FLOAT_TIME);
  Serial.print("Max Float Altitude: "); Serial.println(MAXFLOATALT);
  Serial.println("\n***************************** \n");
}

void parseAndSave(char *buff)
{
  char *name = strtok(buff, " =");
  if (name)
  {
    char *junk = strtok(NULL, " ");
    if (junk)
    {
      char *valu = strtok(NULL, " ");
      if (valu)
      {
        int val = atoi(valu);
        if (strcmp(name, "FLIGHT_NUMBER") == 0)
        {
          flightNum = val;
        }
        if (strcmp(name, "RELEASE_POS") == 0)
        {
          RELEASE_POS = val;
        }
        if (strcmp(name, "OPEN_POS") == 0)
        {
          OPEN_POS = val;
        }
        if (strcmp(name, "CLOSED_POS") == 0)
        {
          CLOSED_POS = val;
        }
        if (strcmp(name, "VENT_ALT") == 0)
        {
          //VENT_ALT_param = val;
          VENT_ALT = val;
        }
        if (strcmp(name, "FLOAT_TIME") == 0)
        {
          FLOAT_TIME = val;
        }
        if (strcmp(name, "MAXFLOATALT") == 0)
        {
          MAXFLOATALT = val;
        }
      }
    }
  }
}