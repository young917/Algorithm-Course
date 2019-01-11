COMPRESSION PROGRAM
===

TERM
---

2018.12.04 ~ 2018.12.11

GOAL
---

Design and implement a Huffman Coding-based file compression utility program.

Focus on Correctness of Implementation and Performance of Program(Compression ratio).

Don't need to focus on time complexity.


EXECUTION
---

*Compress*

`mp3_20171697 -c input.txt`

Then, output file "input.txt.zz" is created.



*Decompress*

`mp3_20171697 -d input.txt.zz`

Then, output file "input.txt.zz.yy" is created.

---

COMPRESSED FILE COMPOTISION
---

* **the number of original characters** : be written for reading right original characters.

* **original characters** : be written according to the order of visiting the Huffman Tree as DFS first with left child.

* **length of encoded key about Huffman Tree composition & remainder after the length is divided by 8** : be written for reading right encoded key.

* **encoded key about Huffman Tree composition** 

* **length of encoded key about original text & remainder after the length is divided by 8** : be written for reading right encoded key.

* **encoded key about original text**

---

ENCODING METHOD
---

* *Build Huffman Tree*

    ...refered to Wikipedia
    
    1. The leaf nodes containing the probabilities of the symbol they represent.

    2. Take the two nodes with smallest probability, and create a new internal node having these two nodes as children. The weight of the new node is set to the sum of the weight of the children. 

    3. Apply the above process again, on the new internal node and on the remaining nodes. Repeat this process until only one node remains, which is the root of the Huffman tree.

* *Encoded Key About Huffman Tree Composition*

    While visiting Huffman Tree as DFS with left child priority, the node is represented as 0 and 1 in order of visiting.

    If the node has children, it is represented as 1. If not, it is represented as 0.

    So the composition of Huffman Tree is encoded to binary number.

* *Encoded Key About Original Text*

    Convert each character into Huffman Code.

* *Write Binary Encoded key on the file*

    After dividing binary number into 8 digits, convert 8 digits into one ASCII code.

