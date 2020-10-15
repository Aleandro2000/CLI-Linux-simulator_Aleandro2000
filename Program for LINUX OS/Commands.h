//Realized by Aleandro 2000
#ifndef COMMANDS_FILE
#define COMMANDS_FILE
/*################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

/*################################################*/

struct directory
{
	char name[255];
	unsigned long long index;
	struct directory *next,*backward,*dir;
};

struct file
{
	char name[255],text[500];
	struct directory *dir;
	struct file *next,*backward;
};

/*################################################*/

//mkdir command

void create(char name_dir[],struct directory **first_dir, struct directory **last_dir, struct directory *current, struct directory **new)
{
	for(struct directory *i=*first_dir;i;i=i->next)
		if(!strcmp(i->name,name_dir))
		{
			printf("mkdir: cannot create directory ‘%s’: File exists\n",name_dir);
			return;
		}
	if(!*first_dir)
	{
		*first_dir=malloc(sizeof(struct directory));
		strcpy((*first_dir)->name,name_dir);
		(*first_dir)->next=(*first_dir)->backward=NULL;
		(*first_dir)->dir=current;
		*last_dir=*first_dir;
		*new=*last_dir;
	}
	else
	{
		struct directory *p;
		p=malloc(sizeof(struct directory));
		strcpy(p->name,name_dir);
		p->dir=current;
		p->next=NULL;
		(*last_dir)->next=p;
		p->backward=*last_dir;
		if(!current)
			p->index=0;
		else
			p->index=current->index+1;
		*last_dir=p;
		*new=*last_dir;
	}
}
void create_root(char name_dir[],struct directory **first_dir, struct directory **last_dir, struct directory *current)
{
	for(struct directory *i=*first_dir;i;i=i->next)
		if(!strcmp(i->name,name_dir))
		{
			printf("mkdir: cannot create directory ‘%s’: File exists\n",name_dir);
			return;
		}
	if(!*first_dir)
	{
		*first_dir=malloc(sizeof(struct directory));
		strcpy((*first_dir)->name,name_dir);
		(*first_dir)->next=(*first_dir)->backward=NULL;
		(*first_dir)->dir=current;
		*last_dir=*first_dir;
	}
	else
	{
		struct directory *p;
		p=malloc(sizeof(struct directory));
		strcpy(p->name,name_dir);
		p->dir=current;
		p->next=NULL;
		(*last_dir)->next=p;
		p->backward=*last_dir;
		if(!current)
			p->index=0;
		else
			p->index=current->index+1;
		*last_dir=p;
	}
}
void create_dir(char dir[], struct  directory **first_dir, struct  directory **last_dir, struct directory *current)
{
	char *p;
	next:
	p=strtok(dir,"/");
	if(!strcmp(p,"../"))
	{
		puts("mkdir: cannot create directory ‘../’: File exists");
		goto next;
	}
	else if(!strcmp(p,"./"))
	{
		puts("mkdir: cannot create directory ‘./’: File exists");
		goto next;
	}
	while(p)
	{
		create(p,first_dir,last_dir,current,&current);
		p=strtok(NULL,"/");
	}
}
void create_root_dir(char dir[], struct  directory **first_dir, struct  directory **last_dir, struct directory *current)
{
	char *p;
	next:
	p=strtok(dir," ");
	if(!strcmp(p,".."))
	{
		puts("mkdir: cannot create directory ‘..’: File exists");
		goto next;
	}
	else if(!strcmp(p,"."))
	{
		puts("mkdir: cannot create directory ‘.’: File exists");
		goto next;
	}
	while(p)
	{
		create_root(p,first_dir,last_dir,current);
		p=strtok(NULL," ");
	}
}

//rmdir command

void rmdir_root(char dir[], struct directory **first_dir, struct directory **last_dir, struct file **first_file, struct directory *current)
{
	bool ok=false;
	struct directory *p,*q;
	struct file *g;
	for(p=*first_dir;p&&!ok;p=p->next)
		if(!strcmp(p->name,dir)&&p->dir==current)
			ok=true;
	if(!ok)
	{
		printf("rmdir: failed to remove '%s': No such file or directory\n",dir);
		return;
	}
	if(!(*first_dir)->next&&!strcmp((*first_dir)->name,dir)&&(*first_dir)->dir==current)
	{
		p=*first_dir;
		*first_dir=(*first_dir)->next;
		free(p);
		return;
	}
	if(!strcmp((*last_dir)->name,dir)&&(*last_dir)->dir==current)
	{
		p=*last_dir;
		*last_dir=(*last_dir)->backward;
		(*last_dir)->next=NULL;
		free(p);
		return;
	}
	p=*first_dir;
	while(p)
		if(!strcmp(p->name,dir)&&p->dir==current)
		{
			g=*first_file;
			while(g)
				if(g->dir==p)
				{
					printf("rmdir: failed to remove '%s': Directory not empty\n",dir);
					return;
				}
				else
					g=g->next;
			q=*first_dir;
			while(q)
				if(q->dir==p)
				{
					printf("rmdir: failed to remove '%s': Directory not empty\n",dir);
					return;
				}
				else
					q=q->next;
			q=p->next;
			strcpy(p->name,p->next->name);
			p->dir=p->next->dir;
			p->next=p->next->next;
			if(!p->next)
				*last_dir=p;
			free(q);
		}
		else
			p=p->next;
}
void rmdir(char *dir, struct directory **first_dir, struct directory **last_dir, struct file **first_file, struct directory *current)
{
	bool ok;
	struct directory *p,*q;
	struct file *g;
	char *a,*b;
	a=malloc(sizeof(dir));
	b=strtok(dir,"/");
	while(b)
	{
		ok=false;
		strcpy(a,b);
		for(p=*first_dir;p&&!ok;p=p->next)
			if(!strcmp(p->name,a)&&p->dir==current)
			{
				ok=true;
				current=p;
			}
		if(!ok)
		{
			printf("rmdir: failed to remove '%s': No such file or directory\n",a);
			free(a);
			return;
		}
		b=strtok(NULL,"/");
	}
	if(!(*first_dir)->next&&!strcmp((*first_dir)->name,a)&&(*first_dir)->dir==current->dir)
	{
		p=*first_dir;
		*first_dir=(*first_dir)->next;
		free(p);
		free(a);
		return;
	}
	if(!strcmp((*last_dir)->name,a)&&(*last_dir)->dir==current->dir)
	{
		p=*last_dir;
		*last_dir=(*last_dir)->backward;
		(*last_dir)->next=NULL;
		free(p);
		free(a);
		return;
	}
	p=*first_dir;
	while(p)
		if(!strcmp(p->name,a)&&p->dir==current->dir)
		{
			g=*first_file;
			while(g)
				if(g->dir==p)
				{
					printf("rmdir: failed to remove '%s': Directory not empty\n",dir);
					free(a);
					return;
				}
				else
					g=g->next;
			q=*first_dir;
			while(q)
				if(q->dir==p)
				{
					printf("rmdir: failed to remove '%s': Directory not empty\n",dir);
					free(a);
					return;
				}
				else
					q=q->next;
			q=p->next;
			strcpy(p->name,p->next->name);
			p->dir=p->next->dir;
			p->next=p->next->next;
			if(!p->next)
				*last_dir=p;
			free(q);
		}
		else
			p=p->next;
	free(a);
}
//pwd command

void pwd(struct directory *first_dir, struct directory *current)
{
	for(struct directory *p=first_dir;p&&p!=current;p=p->next)
			printf("/%s",p->name);
	if(current)
		printf("/%s\n",current->name);
	else
		printf("\n");
}

//cd command

void cd(char way[], struct directory *first_dir, struct directory **current)
{
	struct directory *p;
	for(p=first_dir;p;p=p->next)
		if(!strcmp(p->name,way)&&p->dir==*current)
			break;
	if(p)
		*current=p;
	else
		printf("bash: cd: %s: No such file or directory\n",way);
}

//ls command

void show_file(struct directory *d, struct file *first_file)
{
	for(struct file *p=first_file;p;p=p->next)
		if(p->dir==d)
			if(strchr(p->name,'.'))
				printf("\033[1;32m%s\033[0m  ",p->name);
			else
				printf("%s  ",p->name);
}
void show_dir(struct directory *current, struct directory *first_dir)
{
	for(struct directory *p=first_dir;p;p=p->next)
		if(p->dir==current)
			printf("\033[1;34m%s\033[0m  ",p->name);
}
void ls(char *name_dir, struct directory *first_dir, struct directory *current, struct file *first_file)
{
	char *p,*q;
	q=malloc(sizeof(name_dir));
	p=strtok(name_dir,"/");
	while(p)
	{
		strcpy(q,p);
		for(struct directory *i=first_dir;i;i=i->next)
			if(!strcmp(i->name,q)&&i->dir==current)
			{
				current=i;
				break;
			}
		p=strtok(NULL,"/");
	}
	for(struct directory *i=first_dir;i;i=i->next)
		if(!strcmp(i->name,q)&&i->dir==current->dir)
		{
			show_dir(i,first_dir);
			show_file(i,first_file);
			free(q);
			return;
		}
	printf("ls: cannot access '%s': No such file or directory",name_dir);
	free(q);
}

//tree command

void tree(struct directory *first_dir, struct file *first_file)
{
	printf("\033[1;32m.\033[0m\n");
	for(struct file *p=first_file;p;p=p->next)
		if(p->dir==NULL)
			if(!strchr(p->name,'.'))
				printf("\033[1;32m%s\033[0m\n",p->name);
			else
				printf("%s\n",p->name);				
	for(struct directory *p=first_dir;p;p=p->next)
	{
		for(unsigned long long i=1;i<=p->index;++i)
			printf("\t");
		printf("\033[1;34m%s\033[0m\n",p->name);
		for(struct file *q=first_file;q;q=q->next)
			if(q->dir==p)
			{
				for(unsigned long long i=1;i<=q->dir->index;++i)
					printf("\t");
				if(!strchr(p->name,'.'))
					printf("\t\033[1;32m%s\033[0m\n",q->name);
				else
					printf("\t%s\n",q->name);
			}
	}
}

//touch command

void touch_root(char name_file[],struct file **first_file, struct file **last_file, struct directory *current)
{
	for(struct file *i=*first_file;i;i=i->next)
		if(!strcmp(i->name,name_file)&&i->dir==current)
		{
			printf("touch: cannot create directory ‘%s’: File exists\n",name_file);
			return;
		}
	if(!*first_file)
	{
		(*first_file)=malloc(sizeof(struct file));
		strcpy((*first_file)->name,name_file);
		(*first_file)->dir=current;
		(*first_file)->next=(*first_file)->backward=NULL;
		*last_file=*first_file;
	}
	else
	{
		struct file *p;
		p=malloc(sizeof(struct file));
		strcpy(p->name,name_file);
		p->dir=current;
		p->next=NULL;
		(*last_file)->next=p;
		p->backward=*last_file;
		*last_file=p;
	}
}
void touch(char *name_file,struct file **first_file, struct file **last_file, struct directory *first_dir, struct directory *current)
{
	bool ok;
	char *a,*b;
	a=malloc(sizeof(name_file));
	b=strtok(name_file,"/");
	while(b)
	{
		ok=false;
		strcpy(a,b);
		b=strtok(NULL,"/");
		for(struct directory *i=first_dir;i&&!ok&&b;i=i->next)
			if(!strcmp(i->name,a)&&i->dir==current)
			{
				ok=true;
				current=i;
			}
		if(!ok&&b)
		{
			printf("touch: cannot touch '%s': No such file or directory\n",name_file);
			free(a);
			return;
		}
	}
	for(struct file *i=*first_file;i;i=i->next)
		if(!strcmp(i->name,name_file)&&i->dir==current)
		{
			printf("touch: cannot create directory ‘%s’: File exists\n",name_file);
			free(a);
			return;
		}
	if(!*first_file)
	{
		(*first_file)=malloc(sizeof(struct file));
		strcpy((*first_file)->name,a);
		(*first_file)->dir=current;
		(*first_file)->next=(*first_file)->backward=NULL;
		*last_file=*first_file;
		free(a);
	}
	else
	{
		struct file *p;
		p=malloc(sizeof(struct file));
		strcpy(p->name,a);
		p->dir=current;
		p->next=NULL;
		(*last_file)->next=p;
		p->backward=*last_file;
		*last_file=p;
		free(a);
	}
}

//echo command

void basic_touch(char name_file[],struct file **first_file, struct file **last_file, struct directory *current)
{
	for(struct file *i=*first_file;i;i=i->next)
		if(!strcmp(i->name,name_file)&&i->dir==current)
			return;
	if(!*first_file)
	{
		(*first_file)=malloc(sizeof(struct file));
		strcpy((*first_file)->name,name_file);
		(*first_file)->dir=current;
		(*first_file)->next=(*first_file)->backward=NULL;
		*last_file=*first_file;
	}
	else
	{
		struct file *p;
		p=malloc(sizeof(struct file));
		strcpy(p->name,name_file);
		p->dir=current;
		p->next=NULL;
		(*last_file)->next=p;
		p->backward=*last_file;
		*last_file=p;
	}
}
void echo(char *message, struct file **first_file, struct file **last_file, struct directory *current)
{
	if(!strchr(message,'>'))
		puts(message);
	else
	{
		char *mesg,*p,*q;
		q=malloc(sizeof(message));
		p=mesg=strtok(message,">");
		p=strtok(NULL,">");
		while(p)
		{
			strcpy(q,p);
			basic_touch(q,first_file,last_file,current);
			p=strtok(NULL,">");
		}
		for(struct file *i=*first_file;i;i=i->next)
			if(!strcmp(i->name,q)&&i->dir==current)
			{
				strcpy(i->text,mesg);
				break;
			}
		free(q);
	}
}

//rm command and subprograms for find command

void rm_root(char name_file[], struct file **first_file, struct file **last_file, struct directory *current)
{
	bool ok;
	for(struct file *p=*first_file;p&&!ok;p=p->next)
		if(!strcmp(p->name,name_file)&&p->dir==current)
			ok=true;
	if(!ok)
	{
		printf("rm: failed to remove '%s': No such file or directory\n",name_file);
		return;
	}
	struct file *p=*first_file,*q,*r;
	if(!(*first_file)->next&&!strcmp((*first_file)->name,name_file)&&(*first_file)->dir==current)
	{
		*first_file=(*first_file)->next;
		free(p);
		return;
	}
	if(!strcmp((*last_file)->name,name_file)&&(*last_file)->dir==current)
	{
		q=*last_file;
		*last_file=(*last_file)->backward;
		(*last_file)->next=NULL;
		free(q);
		return;
	}
	while(p)
		if(!strcmp(p->name,name_file)&&p->next->dir==current)
		{
			q=p->next;
			strcpy(p->name,p->next->name);
			strcpy(p->text,p->next->text);
			p->dir=p->next->dir;
			p->next=p->next->next;
			if(!p->next)
				*last_file=p;
			free(q);
		}
		else p=p->next;
}
void rm(char *name_file, struct file **first_file, struct file **last_file, struct directory *first_dir, struct directory *current)
{
	bool ok;
	char *a,*b;
	a=malloc(sizeof(name_file));
	b=strtok(name_file,"/");
	while(b)
	{
		ok=false;
		strcpy(a,b);
		b=strtok(NULL,"/");
		for(struct directory *i=first_dir;i&&!ok&&b;i=i->next)
			if(!strcmp(i->name,a)&&i->dir==current)
			{
				ok=true;
				current=i;
			}
		if(!ok&&b)
		{
			printf("rmdir: failed to remove '%s': No such file or directory\n",a);
			free(a);
			return;
		}
	}
	struct file *p=*first_file,*q,*r;
	if(!(*first_file)->next&&!strcmp((*first_file)->name,a)&&(*first_file)->dir==current)
	{
		*first_file=(*first_file)->next;
		free(p);
		free(a);
		return;
	}
	if(!strcmp((*last_file)->name,a)&&(*last_file)->dir==current)
	{
		q=*last_file;
		*last_file=(*last_file)->backward;
		(*last_file)->next=NULL;
		free(q);
		free(a);
		return;
	}
	while(p)
		if(!strcmp(p->name,a)&&p->next->dir==current)
		{
			q=p->next;
			strcpy(p->name,p->next->name);
			strcpy(p->text,p->next->text);
			p->dir=p->next->dir;
			p->next=p->next->next;
			if(!p->next)
				*last_file=p;
			free(q);
		}
		else p=p->next;
	free(a);
}

//wc fuction

void wc_root(char name_file[], struct file *first_file, struct directory *current)
{
	struct file *q;
	for(q=first_file;q;q=q->next)
		if(!strcmp(q->name,name_file)&&q->dir==current)
			break;
	if(!q)
		printf("wc: %s: No such file or directory\n",name_file);
	else
	{
		char *p;
		unsigned int count=0;
		p=strtok(q->text,"\n");
		while(p)
		{
			++count;
			p=strtok(NULL,"\n");
		}
		printf("%d ",count);
		count=0;
		p=strtok(q->text," .,';|\\}{+=-_)(!@#$%^&*/~<>?\n\t\r");
		while(p)
		{
			++count;
			p=strtok(NULL," .,';|\\}{+=-_)(!@#$%^&*/~<>?\n\t\r");
		}
		printf("%d %ld %s\n",count,sizeof(q->name),name_file);
	}
}
void wc(char *name_file, struct file *first_file, struct directory *first_dir, struct directory *current)
{
	struct file *q;
	bool ok;
	char *a,*b;
	a=malloc(sizeof(name_file));
	b=strtok(name_file,"/");
	while(b)
	{
		ok=false;
		strcpy(a,b);
		b=strtok(NULL,"/");
		for(struct directory *i=first_dir;i&&!ok&&b;i=i->next)
			if(!strcmp(i->name,a)&&i->dir==current)
			{
				ok=true;
				current=i;
			}
		if(!ok&&b)
		{
			printf("wc: %s: No such file or directory\n",name_file);
			free(a);
			return;
		}
	}
	for(q=first_file;q;q=q->next)
		if(!strcmp(q->name,a)&&q->dir==current)
			break;
	if(!q)
	{
		printf("wc: %s: No such file or directory\n",name_file);
		free(a);
	}
	else
	{
		char *p;
		unsigned int count=0;
		p=strtok(q->text,"\n");
		while(p)
		{
			++count;
			p=strtok(NULL,"\n");
		}
		printf("%d ",count);
		count=0;
		p=strtok(q->text," .,';|\\}{+=-_)(!@#$%^&*/~<>?\n\t\r");
		while(p)
		{
			++count;
			p=strtok(NULL," .,';|\\}{+=-_)(!@#$%^&*/~<>?\n\t\r");
		}
		printf("%d %ld %s\n",count,sizeof(q->name),name_file);
		free(a);
	}
}

//cat command

void cat_root(char file_name[], struct file *first_file, struct directory *current)
{
	bool ok=false;
	for(struct file *p=first_file;p&&!ok;p=p->next)
		if(p->dir==current&&!strcmp(p->name,file_name))
		{
			puts(p->text);
			ok=true;
		}
	if(!ok)
		printf("cat: %s: No such file or directory\n",file_name);
}
void cat(char *file_name, struct file *first_file, struct directory *first_dir, struct directory *current)
{
	bool ok;
	char *a,*b;
	a=malloc(sizeof(file_name));
	b=strtok(file_name,"/");
	while(b)
	{
		ok=false;
		strcpy(a,b);
		b=strtok(NULL,"/");
		for(struct directory *i=first_dir;i&&!ok&&b;i=i->next)
			if(!strcmp(i->name,a)&&i->dir==current)
			{
				ok=true;
				current=i;
			}
		if(!ok&&b)
		{
			printf("cat: %s: No such file or directory\n",file_name);
			free(a);
			return;
		}
	}
	ok=false;
	for(struct file *p=first_file;p&&!ok;p=p->next)
		if(p->dir==current&&!strcmp(p->name,a))
		{
			puts(p->text);
			ok=true;
		}
	if(!ok)
	{
		printf("cat: %s: No such file or directory\n",a);
		free(a);
	}
	else
		free(a);
}

//date command [STANDARD]

void date()
{
	time_t now;
	now=time(NULL);
	printf(ctime(&now));
}

//find function

void directory_root(struct directory *root)
{
	if(root)
	{
		directory_root(root->dir);
		printf("/%s",root->name);
	}
}
void find(char name_file[], struct file *first_file)
{
	struct file *p;
	bool ok;
	if(!first_file)
		printf("find: ‘%s’: No such file or directory\n",name_file);
	else
		for(p=first_file;p&&!ok;p=p->next)
		{
			ok=false;
			if(!strcmp(p->name,name_file))
				ok=true;
			if(ok)
			{
				directory_root(p->dir);
				printf("\n");
			}
			else
				printf("find: ‘%s’: No such file or directory\n",name_file);
		}
}
//#################################################
void remove_all(struct directory **first_dir, struct file **first_file)
{
	struct file *q;
	while(*first_file)
	{
		q=*first_file;
		(*first_file)=(*first_file)->next;
		free(q);
	}
	struct directory *p;
	while(*first_dir)
	{
		p=*first_dir;
		(*first_dir)=(*first_dir)->next;
		free(p);
	}
}
void man(char command[])
{
	if(!strcmp(command,"cd"))
	{
		char pause;
		system("@cls||clear");
		printf("cd - open a directory\n\n");
		printf("\tExample of usage:\n\n");
		printf("\tcd DIRECTORY");
		pause=getchar();
		system("@cls||clear");
	}
	else if(!strcmp(command,"pwd"))
	{
		char pause;
		system("@cls||clear");
		printf("pwd - show the current directoy");
		pause=getchar();
		system("@cls||clear");
	}
	else if(!strcmp(command,"rm"))
	{
		char pause;
		system("@cls||clear");
		printf("rm - remove the files from current directoy\n\n");
		printf("\tExample of usage:\n\n");
		printf("\trmdir DIRECTORY");
		pause=getchar();
		system("@cls||clear");
	}
	else if(!strcmp(command,"man"))
	{
		char pause;
		system("@cls||clear");
		printf("\tman - manual for users :)\n\n");
		pause=getchar();
		system("@cls||clear");
	}
	else if(!strcmp(command,"rmdir"))
	{
		char pause;
		system("@cls||clear");
		printf("rmdir - remove a directory from the current directory\n\n");
		printf("\tExample of usage:\n\n");
		printf("\trmdir DIRECTORY");
		pause=getchar();
		system("@cls||clear");
	}
	else if(!strcmp(command,"hostname"))
	{
		char pause;
		system("@cls||clear");
		printf("hostname - show or change the hostname\n\n");
		printf("\tExample of usage:\n\n");
		printf("\thostname [NAME]");
		pause=getchar();
		system("@cls||clear");
	}
	else if(!strcmp(command,"ls"))
	{
		char pause;
		system("@cls||clear");
		printf("ls - show the content from a directory\n\n");
		printf("\tExample of usage:\n\n");
		printf("\tls [DIRECTORY] => show the content from the directory\n");
		printf("\tls => show the content from current directory");
		pause=getchar();
		system("@cls||clear");
	}
	else if (!strcmp(command,"cat"))
	{
		char pause;
		system("@cls||clear");
		printf("cat - show the text from a file\n\n");
		printf("\tExample of usage:\n\n");
		printf("\tcat [FILE_NAME]");
		pause=getchar();
		system("@cls||clear");
	}
	else if (!strcmp(command,"tree"))
	{
		char pause;
		system("@cls||clear");
		printf("tree - show the content from all directories\n\n");
		printf("\tExample of usage:\n\n");
		printf("\ttree");
		pause=getchar();
		system("@cls||clear");
	}
	else if (!strcmp(command,"echo"))
	{
		char pause;
		system("@cls||clear");	
    	printf("echo - display a line of text\n\n");
    	printf("\tExample of usage:\n\n");
    	printf("\techo [SHORT-OPTION]... [STRING]...\n");
    	printf("\techo LONG-OPTION");
    	pause=getchar();
    	system("@cls||clear");
	}
	else if (!strcmp(command,"touch"))
	{
		char pause;
		system("@cls||clear");
		printf("touch - create files in current directory\n\n");
		printf("\tExample of usage:\n\n");
		printf("\ttouch [FILE_NAMES]");
		pause=getchar();
		system("@cls||clear");
	}
	else if (!strcmp(command,"mkdir"))
	{
		char pause;
		system("@cls||clear");
		printf("mkdir - create directories\n\n");
		printf("\tExample of usage:\n\n");
		printf("\tmkdir [DIRECTORY_NAMES]");
		pause=getchar();
		system("@cls||clear");
	}
	else if (!strcmp(command,"wc"))
	{
		char pause;
		system("@cls||clear");
		printf("wc - print newline, word, and byte counts for each file\n\n");
		printf("\tExample of usage:\n\n");
		printf("\twc [FILE]");
		pause=getchar();
		system("@cls||clear");
	}
	else if (!strcmp(command,"date"))
	{
		char pause;
		system("@cls||clear");
		printf("date - in this version print the current date\n\n");
		printf("\tUsage:\n\n");
		printf("\tdate");
		pause=getchar();
		system("@cls||clear");
	}
	else if (!strcmp(command,"reboot"))
	{
		char pause;
		system("@cls||clear");
		printf("reboot - restart the PC\n\n");
		printf("\tUsage:\n\n");
		printf("\treboot");
		pause=getchar();
		system("@cls||clear");
	}
	else if (!strcmp(command,"find"))
	{
		char pause;
		system("@cls||clear");
		printf("find - search for files in a directory hierarchy\n\n");
		printf("\tUsage:\n\n");
		printf("\tfind [NAME_FILE]");
		pause=getchar();
		system("@cls||clear");
	}
	else
		printf("*Unknown command!*\n");
}
void logo()
{
	printf("\033[1;31m");
	printf("  :::::::::  :::   :::              :::     :::        ::::::::::     :::     ::::    ::: :::::::::  :::::::::   ::::::::\n");
	printf(" :+:    :+: :+:   :+:            :+: :+:   :+:        :+:          :+: :+:   :+:+:   :+: :+:    :+: :+:    :+: :+:    :+:\n");
   	printf(" +:+    +:+  +:+ +:+            +:+   +:+  +:+        +:+         +:+   +:+  :+:+:+  +:+ +:+    +:+ +:+    +:+ +:+    +:+\n");
 	printf("  +#++:++#+    +#++:            +#++:++#++: +#+        +#++:++#   +#++:++#++: +#+ +:+ +#+ +#+    +:+ +#++:++#:  +#+    +:+\n");
	printf("  +#+    +#+    +#+             +#+     +#+ +#+        +#+        +#+     +#+ +#+  +#+#+# +#+    +#+ +#+    +#+ +#+    +#+\n");    
	printf(" #+#    #+#    #+#             #+#     #+# #+#        #+#        #+#     #+# #+#   #+#+# #+#    #+# #+#    #+# #+#    #+#\n");
	printf("#########     ###             ###     ### ########## ########## ###     ### ###    #### #########  ###    ###  ########   2000\n\n");
	printf("\033[0m");
}
//#################################################
#endif
