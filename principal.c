#include<stdio.h>
#include<stdlib.h>

int disponible=3000;
int memoria[4500];
typedef struct p{
	int id;
	int tama;
	int inicio;
}Proceso;

/*typedef struct queue{
	int id;
	int inicio;
	int tama;
	struct queue *next;
}Cola;
*/

typedef struct cola{
	Proceso encola;
	int tam;
	struct cola *atras;
}Cola;

int num_procs(FILE*);
Proceso* guarda(int ,FILE*);
void procesador(Proceso*,int);
void asigna(Proceso*,int);
int libera(Proceso);
Cola* crea(Proceso);
Cola* encolar(Cola* ,Proceso);


int main(int argc, char* argv[]){
	Proceso *proc;
	int i;
	FILE *archivo;
	int procs;
	if((archivo=fopen(argv[1],"r"))==NULL){
		printf("\nNo se pudo abrir el archivo");
	}
	procs=num_procs(archivo);
	proc=guarda(procs,archivo);
	procesador(proc,procs);
	//printf("id \ttama \t inicio\n ");
	/*for(i=0;i<procs;i++){
		printf("%d \t%d \t %d\n",proc[i].id,proc[i].tama,proc[i].inicio);
	}*/
	/*for(i=1500;i<1542;i++){
		
		printf("\nLocalidad[%d] = %d\n",i,memoria[i]);
	}*/
	fclose(archivo);
	return 0;
}

int num_procs(FILE *archivo){
	int n,tama=0;
	while((n=fgetc(archivo))!=EOF){
		if(n=='\n'){
			tama++;
		}
	}
	return tama+1;
}

Proceso* guarda(int proc,FILE *archivo){
	Proceso *procs;
	int i=0;
	procs=(Proceso*)calloc(proc,sizeof(Proceso));
	rewind(archivo);
	while(!feof(archivo)){
		fscanf(archivo,"%d %d",&procs[i].id,&procs[i].tama);
		procs[0].inicio=1500;
		i++;
	}
	return procs;
}

void procesador(Proceso *procs,int num_procs){
	int i=0;
	int k;
	Cola *inicio=NULL;
	Cola *siguiente;
	
	for(i=0;i<num_procs;i++){
		printf("\nputo2\n");
		if(procs[i].tama!=0){
			printf("\nputo3\n");
			if(procs[i].tama<=disponible){
				printf("\nputo4\n");
				printf("\nid %d tama %d\n",procs[i].id,procs[i].tama);
				asigna(&procs[i],i);
				disponible-=procs[i].tama;
				printf("\nSe asignaron %d localidades",procs[i].tama);
				printf(" de memoria al proceso %d ",procs[i].id);
				printf(",memoria restante... %d\n",disponible);
				printf("id \ttama \t inicio\n ");
				for(k=0;k<num_procs;k++){
					printf("%d \t%d \t %d\n",procs[k].id,procs[k].tama,procs[k].inicio);
				}
			
			}else{
				printf("\nNo hay memoria suficiente");
				printf("\nEl proceso %d entro en cola\n",procs[i].id);
				printf("\nSituacion de la memoria\n");
				printf("id \ttama \t inicio\n ");
				for(k=0;k<num_procs;k++){
					printf("%d \t%d \t %d\n",procs[k].id,procs[k].tama,procs[k].inicio);
				}
				/*if(inicio==NULL)
					inicio= crea(procs[i]);
				else{
					siguiente= encolar(inicio,procs[i]);
				}*/
			}
		}else{
			disponible+=libera(procs[i]);
			printf("\nEl proceso %d ha terminado, ",procs[i].id);
			printf("memoria restante %d\n",disponible);
			//revisar procesos en cola y asignar
		}	
		printf("\nputo\n");
	}
	printf("\nSe atendio a todos los procesos\n");
	printf("id \ttama \t inicio\n ");
	for(k=0;k<num_procs;k++){
		printf("%d \t%d \t %d\n",procs[k].id,procs[k].tama,procs[k].inicio);
	}
	printf("\nMemoria restante %d\n",disponible);
	return;
}

void asigna(Proceso *proc,int i){
	static int k;
	static int sig_ini;
	printf("\nputo5\n");
	if(i==0){
		printf("\nputo6\n");
		for(k=(*proc).inicio;k<proc->inicio+proc->tama;k++){
			memoria[k]=proc->id;
		}
		sig_ini=k;
	}else{
		printf("\nputo7\n");
		printf("\n sig antes %d\n",proc->inicio);
		proc->inicio=sig_ini;
		printf("\n sig desp %d\n",proc->inicio);
		printf("\n tama %d\n",proc->tama);
		for(k=proc->inicio;k<proc->inicio+proc->tama;k++){
			//printf("\nputo8\n");
			memoria[k]=proc->id;
		}	
		sig_ini=k;

	}
	return;
}

int libera(Proceso proc){
	int i;
	int cuanto_borro=0;
	int borra_desde;
	
	for(i=1500;i<4500;i++){
		if(memoria[i]==proc.id){
			borra_desde=i;
			break;
		}
	}
	
	for(i=borra_desde;i<4500;i++){
		if(memoria[i]==proc.id)
			cuanto_borro++;
	}
	
	for(i=borra_desde;i<borra_desde+cuanto_borro;i++){
		memoria[i]=0;
	}
	return cuanto_borro;
}
/*
void crea(Cola *primero,Cola *ultimo,Proceso *proc){
	Cola nuevo;
	nuevo=(Cola)malloc(sizeof(Cola));
	nuevo->next=NULL;
	
	return;
}
*/

Cola* crea(Proceso proc){
	Cola *primero;
	primero=(Cola*)malloc(sizeof(Cola));
	primero->encola=proc;
	primero->tam++; 
	primero->atras= NULL;

	printf("\nid %d tama %d\n",primero->encola.id,primero->encola.tama);
	return primero;
}


Cola* encolar(Cola *siguiente,Proceso proc){
	siguiente->atras= (Cola*)malloc(sizeof(Cola));
	siguiente->encola= proc;
	siguiente->tam++;
	printf("\nid %d tama %d\n",siguiente->encola.id,siguiente->encola.tama);

	return siguiente;
}