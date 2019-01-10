SORTING ALGORITHM
===


TERM
---

2018.11.10 ~ 2018.11.19

GOAL
---

Understand performance of different sorting algorithms and design your own efficient sorting algorithm

<img src="https://user-images.githubusercontent.com/30820487/50997414-d8863680-1567-11e9-830f-75b17f6d1050.png" width="80%" height="80%">

INPUT & OUTPUT
---

*input*: input file name and algorithm index

*output*: "result_algorithm_inputfilename"

<img src="https://user-images.githubusercontent.com/30820487/50997606-5ba78c80-1568-11e9-962a-9b1ea68aec55.png" width="80%" height="80%">

MY OWN ALGORITHM
---

**IDEA**

Tim Sort + Intro Sort

 I want to use Tim Sort's fast performance of nonincreasing or nondecreasing list. So, I design algorithm that before sorting looks over the list and find nonincreasing or nondecreasing subsequence list. In case of nonincreasing subsequence list, swap it then it becomes nondecreasing subsequence list. And then find next nonincreasing or nondecreasing subsequence list that is not overlapped with previous one. Do this until the end. If the number of subsequence list is 1, then ends performance.
 
  If the number of subsequence list is more than 1, do Intro Sort. For doing Intro Sort, set max depth value as 2 * floor(logN) that refered to Wiki. The number of Quick Sort repetitions has reached the limit, then convert to Heap Sort for the rest unsorted list. This avoids leading to O(N^2), bad time complexity of Quick Sort. While doing Quick Sort, between two part that partition with pivot, small part is sorted by recursive Quick Sort and large part is sorted by iterative Quick Sort. This helps to reduce the number of recursive function calling.
