#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]){
    int rank,p,N,choice,root=0;
    double local_sum = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    

    while(1){
        if(rank==0){//Μόνο ο P0 εμφανίζει μηνύματα στην οθόνη 
            
                printf("\nI. C(NxN) + D(NxN)\n");
                printf("II. C(NxN) * B(Nx1)\n");
                printf("III. A(1xN) + B(Nx1)\n");
                printf("IV. C(NxN) * D(NxN)\n");
                printf("V. Exit\n");
                printf("Your choice (1-5):");
                fflush(stdout);  
                
                scanf("%d",&choice);
                
        }   
        MPI_Bcast( &choice ,1 , MPI_INT ,root ,MPI_COMM_WORLD);//Στέλνουμε σε όλους του επεξεργαστές την επιλογή του χρήστη
        switch (choice)
        {
            case 1:{ //Ι. 
                if(rank==0){
                    printf("Please enter N:");
                    fflush(stdout);  
            
                    scanf("%d",&N);
                    while(p > N || N%p!=0){
                        printf("Please enter N:");
                        fflush(stdout);  
            
                        scanf("%d",&N);
                    }
                }   
                MPI_Bcast( &N , 1 , MPI_INT, root , MPI_COMM_WORLD);//Στέλνουμε σε όλους του επεξεργαστές το μέγεθος των πινάκων
                double C[N][N] ; //Οι ολοκληρωμένοι πίνακες C και D
                double D[N][N] ;
                double local_C[N*N/p]; //Οι τοπικοί πίνακες C και D στους οποίους θα χωρίσουμε τα N*N/p στοιχεία των ολοκληρωμένων 
                //πινάκων που αντιστοιχούν σε κάθε επεξεργαστή (Έστω Ν=4 p=2, τότε οι C,D θα έχουν 4*4=16 στοιχεία, 
                //άρα ο κάθε επεξεργαστής θα πρέπει να πάρει 4*4/2 = 8 στοιχεία και όχι 4/2 = 2 στοιχεία που θα έπαιρναν
                // εάν οι πίνακες ήταν μονοδιάστατοι)
                double local_D[N*N/p];
                
                if(rank==0){
                    for(int i = 0; i<N;i++){
                        for(int j = 0; j<N;j++){
                            printf("Enter C[%d][%d]",i,j);
                            fflush(stdout);  
            
                            scanf("%lf",&C[i][j]);
                            printf("Enter D[%d][%d]",i,j);
                            fflush(stdout);  
            
                            scanf("%lf",&D[i][j]);
                        }
                        
                    }
                }
                MPI_Scatter(C , N*N/p,  MPI_DOUBLE ,local_C , N*N/p ,MPI_DOUBLE , root , MPI_COMM_WORLD);//Χωρίζουμε τους πίνακες D και C
                MPI_Scatter(D , N*N/p,  MPI_DOUBLE ,local_D , N*N/p ,MPI_DOUBLE , root , MPI_COMM_WORLD);//Σε N*N/p ίσα κομμάτια και ο κάθε 
                //επεξεργαστής αποθηκεύει το μερίδιό του στα τοπικά C και D
                local_sum=0;
                for(int i = 0; i<N*N/p;i++){
                   local_sum+=local_C[i]+local_D[i];
                }
                double global_sum = 0;
                MPI_Reduce( &local_sum ,&global_sum ,1 , MPI_DOUBLE, MPI_SUM ,  root , MPI_COMM_WORLD);//Στέλνουμε όλα τα τοπικά αθροίσματα
                //στον root=P0  αθροίζοντάς τα και αποθηκεύοντας το τελικό αποτέλεσμα στο global_sum (globalsum= local_sum_P1+...+local_sum_PN)
                if(rank==0){
                    printf("\nThe total sum is %lf\n",global_sum);
                }
                break;
            }
            case 2:{//II
                 if(rank==0){
                    printf("Please enter N:");
                    fflush(stdout);  
            
                    scanf("%d",&N);
                    while(p > N || N%p!=0){
                        printf("Please enter N:");
                        fflush(stdout);  
            
                        scanf("%d",&N);
                    }
                }   
                MPI_Bcast( &N , 1 , MPI_INT, root , MPI_COMM_WORLD);//Στέλνουμε σε όλους του επεξεργαστές το μέγεθος των πινάκων
                double C[N][N];
                double B[N];
                double local_C[N*N/p];//Ο τοπικός πίνακας C του κάθε επεξεργαστή θα έχει N*N/p στοιχεία (Έστω Ν=4 p=2, τότε ο C θα έχει 4*4=16 στοιχεία, 
                //άρα ο κάθε επεξεργαστής θα πρέπει να πάρει 4*4/2 = 8 στοιχεία)
                
                if(rank ==0){
                    for(int i=0; i<N;i++){
                        
                        for(int j=0;j<N;j++){
                            printf("Δώστε το C[%d][%d]",i,j);
                            fflush(stdout);  
                            scanf("%lf",&C[i][j]);
                        }
                    }
                    for(int i=0; i<N;i++){
                        printf("Δώστε το Β[%d]",i);
                        fflush(stdout);  
                        scanf("%lf",&B[i]);
                        
                    }
                }
                MPI_Scatter(C , N*N/p , MPI_DOUBLE ,local_C ,N*N/p, MPI_DOUBLE , root , MPI_COMM_WORLD);
                MPI_Bcast(B,N,MPI_DOUBLE,root,MPI_COMM_WORLD);
                double temp_sum[N] ;double tmpsum=0;
                for(int i = 0 ; i<N; i++){temp_sum[i]=0;}
                double local_prod;
                for(int i =0; i<N/p;i++){//i loop N/p but j loop runs N times, so C will get parsed N*N/p times
                    temp_sum[i]=0;
                    for(int j=0;j<N;j++){
                       temp_sum[i]+=local_C[i*N+j]*B[j];
                       
                    }
                    
                    
                }
                local_prod=1;
                for(int i=0;i<N/p;i++){
                    local_prod*=temp_sum[i];
                }
                /* double final_prod=1;
                MPI_Reduce( &local_prod ,&final_prod ,1 , MPI_DOUBLE ,MPI_PROD ,  root , MPI_COMM_WORLD); */
                double final[N],final_prod;
                MPI_Allgather( temp_sum , N/p ,MPI_DOUBLE,final,N/p , MPI_DOUBLE , MPI_COMM_WORLD);
                MPI_Reduce( &local_prod , &final_prod,1 , MPI_DOUBLE ,MPI_PROD, root,MPI_COMM_WORLD);
                if(rank==0){
                    for(int i=0;i<N;i++){
                        printf("Sum of R[%d] is %lf\n",i,final[i]);
                    }
                    printf("The product of all items is %lf",final_prod);
                }
                break;
            }
            
            case 3:{//TODO:III)
                break;

            }
            case 4: {//TODO:IV)
                break;

            }
                
            case 5:{//TODO:V)
                MPI_Finalize();
                exit(0);
            }

            default:{
                printf("\nPlease enter a valid choice! (1-5);");
                break; 
            }

        }
    
    
            
        
    }

        

    }

    
  

