# SociallyAssistiveRobotics-PTA-Bot
A socially assistive robotics that assists patients in cardiac rehabilitation exercise 

## PTA. BOT
This Project consists of multiple components:
Webots Simulator - OP2.
Face Detection - CNN Algorithm.
Gesture Description Language Studio v1.05 (GDL Studio v1.05).
Gesture Detection Script (Python).
Microsoft Kinect V1.
These 5 components are running on 3 computers at the same time to execute a single experiment.
 
## How to use:
1. Download [Webots](https://www.cyberbotics.com/) on a computer.
2. Clone the [PTA. BOT repository](https://github.com/Yurockkk/SociallyAssistiveRobotics-PTA-Bot) (PBR).
3. Follow the Readme.md on the repository for prerequisites and installation instructions.
4. Import the project into the Webots simulator.
5. Clone the [Face Detection repository](https://github.com/Derekxbj/face-detection) (FDR) on a different computer with a webcam.
6. Follow the Readme.md on the repository for prerequisites and installation instructions.
7. Download [GDL Studio v1.05](http://www.cci.up.krakow.pl/gdl/) application and install on a windows computer.
8. Clone the [Gesture Recognition repository](https://github.com/lalkulaib/gestureRecognition) (GRR) on the same computer (windows).
9. Replace the downloaded GDL application in the Gesture recognition repository folder.
10. Apply changes to the server.py code (IP and port number) in the GRR folder before running.
11. Make sure all components are set up correctly:
    - webcam is facing patient, close enough to detect face.
    - Kinect is far enough to detect skeletal data.
12. Run the Webots simulator, server.py (GRR) and server.py(FDR) at the same time, webots will start initiating contact with the two other computers and the experiment will run.

