Dear reviewer,

In order to perfom an easy review, please follow the next steps:
	
	* download the entire 'projects' directory in my git repo
	* extract the directory
	* go inside 'projects' directory where you can see 'debug, include, src ...'
	* open terminal from this directory
	* here you are ready for the magic to happen..., these are your options :
		(Visual Code is required)(there might be multiple test functions, check in the main)
		

		* open all relevant files in visual C : 			<make open>
			(watchdog & wd are relevant int this case)

		* run the executable (creates libs,
			compiles, links, runs, everything...): 			<make deb>
															<./bin/debug/watchdog.out ./bin/debug/wd.out>
			
		* check memory leaks with valgrind : 				<make deb> 
															<vlg ./bin/debug/watchdog.out ./bin/debug/wd.out>

		FOR MORE MAKE FILE OPTIONS -> SEE INTO MAKE FILE
		Note :  this project is a bit different so makefile options may not work.


	Please destroy my code during your review in order to help me improve it !
	 	
	ENJOY !	

