"# sort" 

---

tests results:

standard implementation           : time(s) 0.000737, cpu clocks: 0002067244, vector size: 1000, tests: 10
standard implementation (paralel) : time(s) 0.000692, cpu clocks: 0001941060, vector size: 1000, tests: 10
bouble sort                       : time(s) 1.429901, cpu clocks: 0149693069, vector size: 1000, tests: 10
merge sort                        : time(s) 0.009871, cpu clocks: 0027718250, vector size: 1000, tests: 10
quick sort                        : time(s) 0.013073, cpu clocks: 0036708579, vector size: 1000, tests: 10
threaded quick sort (naive)       : time(s) 0.047403, cpu clocks: 0133107198, vector size: 1000, tests: 10
threaded quick sort               : time(s) 0.023233, cpu clocks: 0065238111, vector size: 1000, tests: 10

---

standard implementation           : time(s) 0.005825, cpu clocks: 0016354241, vector size: 100000, tests: 1
standard implementation (paralel) : time(s) 0.004257, cpu clocks: 0011951958, vector size: 100000, tests: 1
merge sort                        : time(s) 0.185112, cpu clocks: 0519793092, vector size: 1000000, tests: 1
quick sort                        : time(s) 0.356671, cpu clocks: 1001527623, vector size: 1000000, tests: 1
threaded quick sort               : time(s) 0.145260, cpu clocks: 0407889517, vector size: 1000000, tests: 1

----


standard implementation           : time(s) 0.485505, cpu clocks: 1363296394, vector size: 10000000, tests: 1
standard implementation (paralel) : time(s) 0.152076, cpu clocks: 0427028445, vector size: 10000000, tests: 1
threaded quick sort               : time(s) 7.323883, cpu clocks: ----------, vector size: 10000000, tests: 1
merge sort                        : time(s) 1.576557, cpu clocks: 0132000493, vector size: 10000000, tests: 1



---

standard implementation           : time(s) 5.016857, cpu clocks: 0343428443, vector size: 100000000, tests: 10
standard implementation (paralel) : time(s) 1.886497, cpu clocks: 0143317896, vector size: 100000000, tests: 10
merge sort                        : time(s) 16.956909, cpu clocks: 0370344934, vector size: 100000000, tests: 10







