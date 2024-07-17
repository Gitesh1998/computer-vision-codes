### Hough Lines Detection

**Requirements**:
- Ubuntu
- Opencv 4.11.1

**How to run**:
- $:~/`make`
- $:~/`./peopleDetect <cmd-arg>`

**Command-line Arguments**:
{ help h | print help message }
{ camera c | 0 | Capture video from camera (device index starting from 0) }
{ video v | Use video as input }

Here we are using pre-trained people/pedestrian detection SVM model which provided by Opencv. Algorithm used by opencv to train model is HOG Descriptor.