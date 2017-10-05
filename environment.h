#ifndef ENVIRONMENT_H_   /* Include guard */
#define ENVIRONMENT_H_

/* 
	- This function should be responsible for importing environment variables into your project.
	- Typically, this function should add $PATH, $HOME & any other needed variables into your variables table 
	- Any future work on variables added by this function should be done through your variable table

	- You also need to store the path of the directory containing this project into a variable, 
	  you'll need it in printning the history & log into a files beside your executable file
*/
void setup_environment( void );  

#endif // ENVIRONMENT_H_