#ifndef VARIABLES_H_   /* Include guard */
#define VARIABLES_H_

/* 
	- This function should be responsible for getting the value of a variable
	- To execute commands, You must always search for the executable files 
	  in the value returned by lookup_variable("PATH");
	- Your code must be dynamic and can handle any number of variables, However,
	  performance is not an issue since in practice we expect low number of variables,
	  as a tip: simple array lookup would be fine
*/
const char* lookup_variable( const char* key );  

/*
	- This function should be responsible for setting the value of a variable
*/
void set_variable( const char* key , const char* value );


/*
	[OPTIONAL] Doesn't affect the code logic
	- This is a helper function to print all your variables
	- Might help much in the debugging or testing
*/
void print_all_variables( void );

#endif // VARIABLES_H_