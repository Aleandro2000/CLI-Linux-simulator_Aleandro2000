//Realized by Aleandro 2000
#include "Commands.h"
int main()
{
	struct directory *first_dir=NULL,*last_dir=NULL,*current=NULL;
	struct file *first_file=NULL,*last_file=NULL;
	char command[500],*p,user[500]="linuxmint-simulator";
	unsigned int i;
	system("@cls||clear");
	logo();
	while(1)
	{
		printf("\033[1;32m");
		printf("aleandro@%s",user);
		printf("\033[0m:");
		if(current)
		{
			printf("\033[1;34m");
			printf("~");
			directory_root(current);
			printf("\033[0m$ ");
		}
		else
		{
			free(current);
			current=0;
		}
		i=0;
		while((command[i++]=getchar())!='\n');
		command[i-1]=0;
		if(command[0])
			p=strtok(command," ");
		else
			continue;
		if(!strcmp(p,"mkdir"))
		{
			p=strtok(NULL,"");
			if(!p)
			{
				printf("mkdir: missing operand\n");
				printf("Try 'man mkdir' for more information.\n");
			}
			else if(strchr(p,'/'))
				create_dir(p,&first_dir,&last_dir,current);
			else
				create_root_dir(p,&first_dir,&last_dir,current);
		}
		else if(!strcmp(p,"tree"))
			tree(first_dir,first_file);
		else if(!strcmp(p,"man"))
		{
			p=strtok(NULL," ");
			if(!p)
			{
				printf("man: missing operand\n");
				printf("Try 'man man' for more information.\n");
			}
			else
				while(p)
				{
					man(p);
					p=strtok(NULL," ");
				}
		}
		else if(!strcmp(p,"cd"))
		{
			p=strtok(NULL,"/");
			if(!p)
				current=NULL;
			else if(!strcmp(p,".."))
				current=current->dir;
			else if(strcmp(p,"."))
				while(p)
				{
					cd(p,first_dir,&current);
					p=strtok(NULL,"/");
				}
		}
		else if(!strcmp(p,"ls"))
		{
			p=strtok(NULL," ");
			if(p)
			{
				ls(p,first_dir,current,first_file);
				printf("\n");
			}
			else
			{
				show_dir(current,first_dir);
				show_file(current,first_file);
				printf("\n");
			}
		}
		else if(!strcmp(p,"pwd"))
		{
			if(current)
				pwd(first_dir,current);
		}
		else if(!strcmp(p,"rmdir"))
		{
			p=strtok(NULL," ");
			if(!p)
			{
				printf("rmdir: missing operand\n");
				printf("Try 'man rmdir' for more information.\n");
			}
			else if(strchr(p,'/'))
				rmdir(p,&first_dir,&last_dir,&first_file,current);
			else
				while(p)
				{
					rmdir_root(p,&first_dir,&last_dir,&first_file,current);
					p=strtok(NULL," ");
				}
		}
		else if(!strcmp(p,"touch"))
		{
			p=strtok(NULL," ");
			if(!p)
			{
				printf("touch: missing operand\n");
				printf("Try 'man touch' for more information.\n");
			}
			else if(strchr(p,'/'))
				touch(p,&first_file,&last_file,first_dir,current);
			else
				while(p)
				{
					touch_root(p,&first_file,&last_file,current);
					p=strtok(NULL," ");
				}
		}
		else if(!strcmp(p,"exit"))
		{
			remove_all(&first_dir,&first_file);
			return 0;
		}
		else if(!strcmp(p,"echo"))
		{
			p=strtok(NULL,"");
			if(!p)
			{
				printf("echo: missing operand\n");
				printf("Try 'man echo' for more information.\n");
			}
			else
				echo(p,&first_file,&last_file,current);
		}
		else if(!strcmp(p,"cat"))
		{
			p=strtok(NULL,"");
			if(!p)
			{
				printf("cat: missing operand\n");
				printf("Try 'man cat' for more information.\n");
			}
			else if(strchr(p,'/'))
				cat(p,first_file,first_dir,current);
			else
				cat_root(p,first_file,current);
		}
		else if(!strcmp(p,"rm"))
		{
			p=strtok(NULL,"");
			if(!p)
			{
				printf("rm: missing operand\n");
				printf("Try 'man rm' for more information.\n");
			}
			else if(strchr(p,'/'))
				rm(p,&first_file,&last_file,first_dir,current);
			else
				while(p)
				{
					rm_root(p,&first_file,&last_file,current);
					p=strtok(NULL," ");
			}
		}
		else if(!strcmp(p,"reboot"))
			system("@shutdown||reboot");
		else if(!strcmp(p,"clear"))
		{
			remove_all(&first_dir,&first_file);
			system("@cls||clear");
		}
		else if(!strcmp(p,"hostname"))
		{
			p=strtok(NULL," ");
			if(!p)
				puts(user);
			else
				strcpy(user,p);
		}
		else if(!strcmp(p,"wc"))
		{
			p=strtok(NULL,"");
			if(!p)
				printf("wc: missing operand\nTry 'man wc' for more information.\n");
			else if(strchr(p,'/'))
				wc(p,first_file,first_dir,current);
			else
				wc_root(p,first_file,current);
		}
		else if(!strcmp(p,"find"))
		{
			p=strtok(NULL," ");
			while(p)
			{
				find(p,first_file);
				p=strtok(NULL," ");
			}
		}
		else if(!strcmp(p,"date"))
			date();
		else
			printf("%s: command not found\n",p);
	}
}
