# Smart Driving Test System  

## Description  
An automated system for monitoring and evaluating driving performance in the practical test, providing real-time assessment and feedback to the driver.  

## Goal  
To provide an automatic and accurate tool for evaluating practical driving, with an emphasis on safety and skill.  

## Key Features  
- Real-time tracking of driver actions  
- Recognition of road signs, traffic lights, pedestrians, and crosswalks  
- Monitoring of lane keeping and vehicle location  
- Automatic response to dangerous situations  
- Costing errors by severity and time  
- Calculating the final score for the examinee based on performance  

## Technologies  
- **Programming Languages:** C++, Python  
- **Computer Vision:** OpenCV, YOLO, Faster R-CNN, LaneNet model, PyTorch  
- **Navigation & Estimation:** Extended Kalman Filter (EKF)  
- **Data Handling & Simulation:** Text-based sensor input files  

## Project Structure  
```
Automated_driving_skills_test_system/
├── API_new/
├── GPS/
│   └── GPS_data_extraction/
├── Lidar/
├── kalman_filter/
├── main_prosses/
│   └── Automated_driving_skills_test_system/
```

## Installation & Usage  

### Required Input Files  
In order to run the system, the following files must be prepared in advance:  
- LiDAR sensor simulation file – `Lidar.txt`  
- IMU sensor simulation file – `IMU.txt`  
- GPS sensor simulation file – `GPS.txt`  
- Video from the vehicle’s front camera (for object recognition)  
- YOLO script – pedestrian and crosswalk detection  
- Faster R-CNN script – object detection in the video  
- LaneNet script – lane detection  
- Dedicated video for lane detection  
- Trained LaneNet model  

### Running Instructions  
1. Make sure that all required files are present in the paths specified in the input section.  
2. Open the project in the **Visual Studio** environment.  
3. Perform a full **Build** of the project.  
4. Click the **Start / Run** button to launch the main program.  

### What Happens During the Run?  
- The system loads the sensor data (LiDAR, IMU, GPS) and the videos.  
- Data fusion is performed using a **Kalman Filter**, while **YOLO**, **Faster R-CNN** and **LaneNet** models are executed on the videos.  
- The output is displayed on screen in real time and includes:  
  - Display of sensor data  
  - Detection of objects in space  
  - Warnings about deviations from traffic laws  
- During the run, temporary text files are created and automatically deleted by the program as needed.  

## Contributing  
1. Fork the repository  
2. Create a new branch  
3. Make your changes  
4. Submit a Pull Request  

## License  
This project was developed as part of an **academic final project**.  
Commercial use or redistribution is **prohibited**.  

## Contact  
- Email: efratShmueli5914@gmail.com  
- GitHub: [EfratSh123](https://github.com/EfratSh123)  
- LinkedIn: *[Add your LinkedIn link here]*  

## Credits  
- [LaneNet Lane Detection (IrohXu)](https://github.com/IrohXu/lanenet-lane-detection-pytorch/tree/main)  
