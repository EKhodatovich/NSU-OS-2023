#include <stdlib.h>
#include <stdio.h>
#include <sys/resource.h>
#include <unistd.h>
#include <errno.h>

extern char **environ;

int main(int argc, char* argv[], char* envp[])
{
	char options[] = "ispcC:uU:dvV:";
	int c = 0;
	printf("argc equals %d\n", argc);
	while ((c = getopt(argc, argv, options)) != EOF) 
	{
		
		switch (c)
		{
			case 'i':
			{
				printf("\nReal user ID: %d\n\
				\rEffective user ID: %d\n\
				\rReal group ID: %d\n\
				\rEffective group ID: %d\n",\
				getuid(), geteuid(), getgid(), getegid());
				break;
			}

			case 's':
			{
				setpgid(0,0);
				break;
			}
			case 'p':
			{
				printf("\nProcess ID: %d\n\
				\rParent process ID: %d\n\
				\rGroup ID: %d\n",getpid(), getppid(), getpgid(0));
				break;
			}
			case 'u':
			{
				struct rlimit curr_limit;
				if (getrlimit(RLIMIT_FSIZE, &curr_limit) == 0)
				{
					printf("File size limit is: %d\n", curr_limit.rlim_cur);
				}
				else
				{
					printf("Cannot get file size limit\n");
				}
				break;
			}
			case 'U':
			{
				errno = 0;
				char* arg = optarg;
				long new_file_limit = atol(arg);
				if (errno !=0 || new_file_limit<=0)
				{
					printf ("Invalid new file limit value");
				}
				else
				{
					struct rlimit new_limit;
					if (getrlimit(RLIMIT_FSIZE, &new_limit) == -1)
					{
						printf("Cannot get file limit");
					}
					else
					{
						new_limit.rlim_cur = new_file_limit;
						if (setrlimit(RLIMIT_FSIZE, &new_limit) == -1)
						{
							printf("Cannot set new file limit");
						}
						else
						{
							printf("New file limit is set: %d", new_file_limit);
						}
					}
				}
				printf("\n");
				break;
			}
			case 'c':
			{
				struct rlimit curr_limit;
				if (getrlimit(RLIMIT_CORE, &curr_limit) == 0)
				{
					printf("Core file limit is: %d\n", curr_limit.rlim_cur);
				}
				else
				{
					printf("Cannot get core file limit\n");
				}
				break;
			}
			case 'C':
			{
				errno = 0;
				char* arg = optarg;
				long new_core_limit = atol(arg);
				if (errno !=0 || new_core_limit<=0)
				{
					printf ("Invalid new core limit value\n");
				}

				else
				{
					struct rlimit new_limit;
					if (getrlimit(RLIMIT_CORE, &new_limit) == -1)
					{
						printf("Cannot get core file limit\n");
					}
					else
					{
						new_limit.rlim_cur = new_core_limit;
						if (setrlimit(RLIMIT_CORE, &new_limit) == -1)
						{
							printf("Cannot set new core file limit\n");
						}
						else
						{
							printf("New core limit is set: %d", new_core_limit);
						}
					}
				}
				printf("\n");
				break;
			}
			case 'd':
			{
				int MAXDIR = 512;
				char* dir= getcwd(NULL, MAXDIR);
				if (dir)
				{
					printf("Current directory is: %s\n", dir);
					free (dir);
				}
				else
				{
					printf("Cannot resolve current directory (%s)\n", dir);
				}
				break;
			}
			case 'v':
			{
				char **p;
				for (p = environ; *p; p++)
				{
					printf ("%s\n", *p);
				}
				break;
			}
			case 'V':
			{
				char* v_arg = optarg;
				if (putenv(v_arg) != 0)
				{
					printf("Cannot put environment variable\n");
				}
				else
				{
					printf("Environmental variable put successfully\n");
				}
				break;
			}
			case '?':
			{
				printf("invalid option is %c\n", optopt);
				break;
			}
		}
	}
}
