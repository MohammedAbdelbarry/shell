
typedef enum{ false = 0 , true = 1 } bool ;

void start_shell(bool read_from_file);
void shell_loop(bool input_from_file);

int main(int argc, char *argv[])
{

    setup_environment();

    // any other early configuration should be here

    if( argc > 1 ){
        start(true);
    }
    else{
        start(false);
    }

    return 0;
}

void start(bool read_from_file)
{
	cd(""); // let shell starts from home

	if(read_from_file){
		// file processing functions should be called from here

		shell_loop(true);
	}
	else{
		shell_loop(false);
	}
}

void shell_loop(bool input_from_file)
{
	bool from_file = input_from_file;

	while(true){
		if(from_file){
			//read next instruction from file

			// if end of file {from_file = false; continue;}
		}
		else{
			//read next instruction from console
		}

		//parse your command here

		//execute your command here

		/*
			you don't need to write all logic here, a better practice is to call functions,
			each one contains a coherent set of logical instructions
		*/
	}
}
