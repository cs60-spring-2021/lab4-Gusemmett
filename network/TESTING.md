# CS60 Spring 2021, Lab 4
###### Angus Emmett
###### GitHub username: gusemmett


## Test 1
Uses given topology and the network stays in stasis

## Test 2
Topology:
```
babylon2.thayer.dartmouth.edu babylon3.thayer.dartmouth.edu 300
babylon3.thayer.dartmouth.edu babylon5.thayer.dartmouth.edu 130
babylon5.thayer.dartmouth.edu babylon6.thayer.dartmouth.edu 70
babylon6.thayer.dartmouth.edu babylon7.thayer.dartmouth.edu 10
babylon7.thayer.dartmouth.edu babylon8.thayer.dartmouth.edu 20
babylon8.thayer.dartmouth.edu babylon2.thayer.dartmouth.edu 50
babylon2.thayer.dartmouth.edu babylon6.thayer.dartmouth.edu 150
babylon8.thayer.dartmouth.edu babylon5.thayer.dartmouth.edu 60
babylon5.thayer.dartmouth.edu babylon2.thayer.dartmouth.edu 40
```

Output of node babylon3 and babylon2 to show convergence:
##### Babylon 3
```
------------------- DV TABLE -------------------
-- From Node: babylon3 to Node: babylon2 -- cost: 300
-- From Node: babylon3 to Node: babylon5 -- cost: 130
-- From Node: babylon3 to Node: babylon3 -- cost: 0
-- From Node: babylon3 to Node: babylon6 -- cost: 380
-- From Node: babylon3 to Node: babylon7 -- cost: 370
-- From Node: babylon3 to Node: babylon8 -- cost: 350
------------------------------------------------

------------------- DV TABLE -------------------
-- From Node: babylon3 to Node: babylon2 -- cost: 300
-- From Node: babylon3 to Node: babylon5 -- cost: 130
-- From Node: babylon3 to Node: babylon3 -- cost: 0
-- From Node: babylon3 to Node: babylon6 -- cost: 380
-- From Node: babylon3 to Node: babylon7 -- cost: 370
-- From Node: babylon3 to Node: babylon8 -- cost: 350
------------------------------------------------

------------------- DV TABLE -------------------
-- From Node: babylon3 to Node: babylon2 -- cost: 170
-- From Node: babylon3 to Node: babylon5 -- cost: 130
-- From Node: babylon3 to Node: babylon3 -- cost: 0
-- From Node: babylon3 to Node: babylon6 -- cost: 200
-- From Node: babylon3 to Node: babylon7 -- cost: 210
-- From Node: babylon3 to Node: babylon8 -- cost: 190
------------------------------------------------

------------------- DV TABLE -------------------
-- From Node: babylon3 to Node: babylon2 -- cost: 170
-- From Node: babylon3 to Node: babylon5 -- cost: 130
-- From Node: babylon3 to Node: babylon3 -- cost: 0
-- From Node: babylon3 to Node: babylon6 -- cost: 200
-- From Node: babylon3 to Node: babylon7 -- cost: 210
-- From Node: babylon3 to Node: babylon8 -- cost: 190
```


##### Babylon2

```
------------------- DV TABLE -------------------
-- From Node: babylon2 to Node: babylon3 -- cost: 300
-- From Node: babylon2 to Node: babylon8 -- cost: 50
-- From Node: babylon2 to Node: babylon6 -- cost: 80
-- From Node: babylon2 to Node: babylon5 -- cost: 40
-- From Node: babylon2 to Node: babylon2 -- cost: 0
-- From Node: babylon2 to Node: babylon7 -- cost: 70
------------------------------------------------

------------------- DV TABLE -------------------
-- From Node: babylon2 to Node: babylon3 -- cost: 170
-- From Node: babylon2 to Node: babylon8 -- cost: 50
-- From Node: babylon2 to Node: babylon6 -- cost: 80
-- From Node: babylon2 to Node: babylon5 -- cost: 40
-- From Node: babylon2 to Node: babylon2 -- cost: 0
-- From Node: babylon2 to Node: babylon7 -- cost: 70
------------------------------------------------

------------------- DV TABLE -------------------
-- From Node: babylon2 to Node: babylon3 -- cost: 170
-- From Node: babylon2 to Node: babylon8 -- cost: 50
-- From Node: babylon2 to Node: babylon6 -- cost: 80
-- From Node: babylon2 to Node: babylon5 -- cost: 40
-- From Node: babylon2 to Node: babylon2 -- cost: 0
-- From Node: babylon2 to Node: babylon7 -- cost: 70
------------------------------------------------
```


## Test3
Edited the given topology to have a shorter path and the nodes converged
```
------------------- DV TABLE -------------------
-- From Node: babylon2 to Node: babylon3 -- cost: 1
-- From Node: babylon2 to Node: babylon4 -- cost: 5
------------------------------------------------

------------------- DV TABLE -------------------
-- From Node: babylon2 to Node: babylon3 -- cost: 1
-- From Node: babylon2 to Node: babylon4 -- cost: 2
------------------------------------------------
```

```
------------------- DV TABLE -------------------
-- From Node: babylon4 to Node: babylon2 -- cost: 5
-- From Node: babylon4 to Node: babylon3 -- cost: 1
------------------------------------------------

------------------- DV TABLE -------------------
-- From Node: babylon4 to Node: babylon2 -- cost: 2
-- From Node: babylon4 to Node: babylon3 -- cost: 1
------------------------------------------------
```