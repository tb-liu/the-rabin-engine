Student Name: Qiaochu Liu

Project Name: NoMoreCrime

What I implemented: Police, randomly patrol and get activated if withness, or received report about crime then capture Ciminal;
		    Detective, exam random people and report to Police if find a Criminal;
		    Civilian, randomly walking if withness crime will either flee or report to police;
		    Criminal, randomly walking and kill civilian, after killed a person will alarms people around him then flee;

Directions (if needed): No restart button, please restart the game if you want;

What I liked about the project and framework: Debug friendly

What I disliked about the project and framework: Because I dont know how to share data between behavior nodes, a small difference would require a new node.

Any difficulties I experienced while doing the project: I dont know how to share data between behavior nodes. 

Hours spent: 12hrs

New selector node (name): none

New decorator nodes (names): D_PoliceInRange, D_IfInRange, D_KeepDoingForAPeriodOfTime, D_KeepTryingUntilFourTimesOrSuccess

10 total nodes (names):L_Report, L_Flee, L_MoveToPolice, L_IfActive, L_Patrol, L_Engage, L_Capture, L_CommitCrime, L_Alarm, L_SetActive, L_Exam, L_ChooseRandomTarget

4 Behavior trees (names): Police, Detective, Civilian, Criminal

Extra credit: