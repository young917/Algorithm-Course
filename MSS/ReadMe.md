Maximum Subsequence Sum
===

TERM
---

2018.9.30 ~ 2018.10.1

GOAL
---

Understand how different algorithms perform differently while producing the same result for the same problem.

<img src="https://user-images.githubusercontent.com/30820487/50996227-ae7f4500-1564-11e9-9306-2d1418c8ba96.png" width="60%" height="60%">

INPUT & OUTPUT
---

input: inputfile name / algorithm index.

output "result_inputfilename":

<img src="https://user-images.githubusercontent.com/30820487/50996402-2188bb80-1565-11e9-831d-ec231832e46c.png" width="60%" height="60%">


ALGORITHM
---
**1. Time complexity O(N^2)**

Compare all possible subsequence.

**2. Time complexity O(NlogN)**

*Divide*

Find left part max subsequence sum.

Find right part max subsequence sum.

*Conquer*

Find mid part max subsequence sum.

compare three and decide max.

**3. Time complexity O(N)**

While searching the data only once, decide max subsequence sum that ends with current index.
