********************************************************************************************
**											  **
**                Diffractive short-pulse mixing  - Version 1.0        			  **
**											  **
********************************************************************************************

This package contains the source code files for the program Mix2dsp which will be 
stored in the default 'C:\SNLO\Mix2dsp\Source' folder.


NOTE:  In order to use these source files, you must have the following software:

	- Visual C++ V5.0 or higher
	- Olectra Chart V6.0 or higher
	- Numerical Recipies for C


The source code for the calculations is contained in the file 'Mix_Prog.cpp' and 
the GUI code is contained in the file 'MixView.cpp', 'GraphDlg.cpp', and '2dWnd.cpp'.

A flowchart diagram (Flowchrt.pdf) for the functions in 'Mix_Prog.cpp' is also included 
in this package.	


********************************************************************************************
**											  **
**   Instructions for creating a "Mix2dsp" workspace with Microsoft Visual C++ V5.0	  **
** 											  **
********************************************************************************************

1.	Open the workspace file for Mix2dsp located in the "C:\SNLO\Mix2dsp\Source" folder.
	
		- Select "File, Open Workspace...".
		- Select "Mix2dsp.dsw" in the "C:\SNLO\Mix2dsp\Source" folder.
		
2.	Now, add the Olectra chart files to the project:

	a. Select "Project, Add To Project, Components and Controls..." from the menu.
	b. Select the "Registered ActiveX Controls" folder.
	c. Select "Olectra Chart 2D Control" from the list, then press "Insert".
	d. Select all of the classes listed, then press "OK".
	e. Repeat steps a-d above for the "Olectra Chart 3D Control".
	
	- Select "Project, Add To Project, Files..." from the menu.
	- Find the "Olectra\Include" folder, then select "2dconst.h" and "3dconst.h".
	- Rename the program and mainframe to avoid confusion with the Sandia version.
		open resource view in Visual C++.
		open String Table folder and modify IDR_MAINFRAME to your new name.
		open Dialog folder and modify IDD_GRAPHDLG to your new name.
		select "Project, Settings..., Settings For: All Configurations, Link Tab, 
			Category General, Output file name" and give the new name to exe 						file.

3.	Compile the project.
