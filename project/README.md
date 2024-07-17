### AV/ADAS Project
I've developed an AV/ADAS project aimed at real-world implementation to provide additional information to drivers for safer driving. This system achieves frame rates close to 20, ensuring timely and efficient data processing for enhanced driver assistance.
#### Features:
1. Lane detection (Region of Interest , Hough Line detection, filtration algorithm)
2. Pedestrain Detection (SVM ML algorithm)
3. Vehicle Detection (Haar-cascade ML algorithm)
4. Traffic single detection (Haar-cascade ML algorithm)

Here we use openmp threading library to achieve parallelism for improving frame rate processing.

**Requirements**:
- Ubuntu
- Opencv 4.1.1  

**How to run**:
- $:~/`make`
- $:~/`./main <video-file-path> --show --store <output-file-name>.avi`
