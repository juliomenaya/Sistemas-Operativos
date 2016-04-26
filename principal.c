/*Elaborado por Julio Méndez Ayala
  Materia: Sistemas Operativos
  PROGRAMA 1; Asignacion de memoria
  Fecha de entrega: 26 de Abril del 2016
  Version del programa: 1.0
  Facultad de Ingenieria, UNAM*/

#include<stdio.h>
#include<stdlib.h>

int disponible=3000; 				//Variable para llevar conteo de memoria disponible
int memoria[4500];					//Arreglo que simula las localidades de memoria

typedef struct p{					//Estructura para guardar los datos de los procesos
	int id;
	int tama;
	int inicio;
}Proceso;

typedef struct cola{				//Cola de procesos
	Proceso encola;
	int tam;
	struct cola *atras;
}Cola;

int num_procs(FILE*);				//Prototipo de funciones
Proceso* guarda(int ,FILE*);		
void procesador(Proceso*,int);
void asigna(Proceso*,int);
int libera(Proceso);
Cola* crea(Proceso);
Cola* encolar(Cola* ,Proceso);

int main(int argc, char* argv[]){
	Proceso *proc;			//Apuntador al primer elementos del arreglo de procesos						
	FILE *archivo;						
	int procs;				//Variable para saber cuantos procesos tiene el archivo
	if(argc<2){									
		printf("\nOlvidaste indicar el nombre del archivo");
		printf(" que contiene a los procesos\n");
		printf("Saliendo del programa...\n");
		return -1;
	}
	if(argc>2){
		printf("\nHay demasiados argumentos\n");
		printf("Saliendo del programa...\n");
		return -1;
	}
	if((archivo=fopen(argv[1],"r"))==NULL){				
		printf("\nNo se pudo abrir el archivo / %s /",argv[1]);
		printf("Saliendo del programa...\n");
		return -1;
	}
	printf("\nEl archivo / %s / se abrio correctamente\n",argv[1]);
	procs=num_procs(archivo);				//Lamadas a funciones 
	proc=guarda(procs,archivo);
	procesador(proc,procs);
	fclose(archivo);
	return 0;
}
//Funcion que calcula el numero de archivos que contenga el archivo
int num_procs(FILE *archivo){
	int n,tama=0;
	while((n=fgetc(archivo))!=EOF){
		if(n=='\n'){
			tama++;
		}
	}
	return tama+1;
}
//Funcion que guarda los datos del archivo en un arreglo dinamico de estructuras del
// tipo Proceso
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

//Funcion que determina si se debe asignar o liberar memoria a los procesos
//Asi como tambien va creando la cola de procesos que no puedan ser asignados
void procesador(Proceso *procs,int num_procs){
	int i=0;
	int k;
	Cola *inicio=NULL;
	Cola *siguiente;
	
	for(i=0;i<num_procs;i++){
		
		if(procs[i].tama!=0){
			
			if(procs[i].tama<=disponible){
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
				if(inicio==NULL)
					inicio= crea(procs[i]);
				else{
					siguiente= encolar(inicio,procs[i]);
				}
			}
		}else{
			disponible+=libera(procs[i]);
			printf("\nEl proceso %d ha terminado, ",procs[i].id);
			printf("memoria restante %d\n",disponible);
			//revisar procesos en cola y asignar
		}	
	}
	printf("\nSe atendio a todos los procesos\n");
	printf("id \ttama \t inicio\n ");
	for(k=0;k<num_procs;k++){
		printf("%d \t%d \t %d\n",procs[k].id,procs[k].tama,procs[k].inicio);
	}
	printf("\nMemoria restante %d\n",disponible);
	return;
}
//Funcion que asigna memoria a los procesos
void asigna(Proceso *proc,int i){
	static int k;
	static int sig_ini;
	
	if(i==0){
		
		for(k=(*proc).inicio;k<proc->inicio+proc->tama;k++){
			memoria[k]=proc->id;
		}
		sig_ini=k;
	}else{
		proc->inicio=sig_ini;
		for(k=proc->inicio;k<proc->inicio+proc->tama;k++){	
			memoria[k]=proc->id;
		}	
		sig_ini=k;
	}
	return;
}

//Funcion que quita de memoria al proceso que es pasado como parametro
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
//Funcion para crear el primer elemento de la cola de procesos
Cola* crea(Proceso proc){
	Cola *primero;
	primero=(Cola*)malloc(sizeof(Cola));
	primero->encola=proc;
	primero->tam++; 
	primero->atras= NULL;
	return primero;
}
//Funcion para agregar elementos a la cola de procesos
Cola* encolar(Cola *siguiente,Proceso proc){
	siguiente->atras= (Cola*)malloc(sizeof(Cola));
	siguiente->encola= proc;
	siguiente->tam++;
	return siguiente;
}