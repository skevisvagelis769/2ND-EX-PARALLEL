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
                    printf("Number of processors are : %d",p);
                    for(int i = 0; i<N;i++){
                        for(int j = 0; j<N;j++){
                            printf("Enter C[%d][%d]",i,j);
                            fflush(stdout);  
            
                            scanf("%lf",&C[i][j]);
                            
                        }
                        
                    }
                     for(int i = 0; i<N;i++){
                        for(int j = 0; j<N;j++){
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
                MPI_Scatter(C , N*N/p , MPI_DOUBLE ,local_C ,N*N/p, MPI_DOUBLE , root , MPI_COMM_WORLD);//Χωρίζουμε το C σε n/p ίσα κομμάτια και το στέλνουμε σε όλους τους επεξεργαστές
                MPI_Bcast(B,N,MPI_DOUBLE,root,MPI_COMM_WORLD);// στέλνουμε σε όλους τους επεξεργαστές το Β γιατί ισχύει ότι γινόμενο = Σc[i,j]*b[j] άρα 
                //χρειαζόμαστε ολόκληρο τον Β σε κάθε επεξεργαστή
                double temp_sum[N] ;double tmpsum=0;
                for(int i = 0 ; i<N; i++){temp_sum[i]=0;}//μηδενίζουμε το temporary sum
                double local_prod;
                for(int i =0; i<N/p;i++){//υπολογίζουμε το τοπικό άθροισμα των n/p στοιχείων του επεξεργαστή
                    temp_sum[i]=0; //με βάση τον τύπο Σc[i,j]*b[j]
                    for(int j=0;j<N;j++){
                       temp_sum[i]+=local_C[i*N+j]*B[j];
                       
                    }
                    
                    
                }
                local_prod=1;
                for(int i=0;i<N/p;i++){//παίρνουμε όλους τους όρους του αθροίσματος και τους 
                    //πολλαπλασιάζουμε για να πάρουμε το γινόμενο όλων των στοιχείων των δύο πινάκων
                    local_prod*=temp_sum[i];
                }

                double final[N],final_prod;
                MPI_Allgather( temp_sum , N/p ,MPI_DOUBLE,final,N/p , MPI_DOUBLE , MPI_COMM_WORLD);//παίρνουμε όλα τα αθροίσματα των επεξεργαστών  
                MPI_Reduce( &local_prod , &final_prod,1 , MPI_DOUBLE ,MPI_PROD, root,MPI_COMM_WORLD);//πολλαπλασιάζουμε όλα τα τοπικά γινόμενα των επεξεργαστών 
                //μεταξύ τους και τα αποθηκεύουμε στο final product 
                if(rank==0){
                    for(int i=0;i<N;i++){
                        printf("Sum of R[%d] is %lf\n",i,final[i]);//τυπώνουμε το αποτέλεσμα στην μορφή πίνακα R[Nx1] που είναι το αποτέλεσμα του C(NxΝ) * Β(Νx1) 
                    }
                    printf("The product of all items is %lf",final_prod);
                }
                break;
            }
            
            case 3:{
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
                MPI_Bcast( &N , 1 , MPI_INT, root , MPI_COMM_WORLD);//Στέλνουμε σε όλους τους επεξεργαστές το μέγεθος των πινάκων
                double A[N];
                double B[N];
                double local_A[N/p],local_B[N/p];
                if(rank ==0){

                    for(int i=0; i<N;i++){

                            printf("Δώστε το A[%d]",i);
                            fflush(stdout);  
                            scanf("%lf",&A[i]);

                    }
                    for(int i=0; i<N;i++){
                        printf("Δώστε το Β[%d]",i);
                        fflush(stdout);  
                        scanf("%lf",&B[i]);
                        
                    }
                }
                
                MPI_Scatter(A ,N/p , MPI_DOUBLE ,local_A ,N/p, MPI_DOUBLE , root , MPI_COMM_WORLD);//Χωρίζουμε το Α σε n/p ίσα κομμάτια και το στέλνουμε σε όλους τους επεξεργαστές
                MPI_Scatter(B,N/p , MPI_DOUBLE ,local_B ,N/p, MPI_DOUBLE , root , MPI_COMM_WORLD);//Χωρίζουμε το B σε n/p ίσα κομμάτια και το στέλνουμε σε όλους τους επεξεργαστές
                
                double temp_sum[N] ;
                for(int i = 0 ; i<N; i++){temp_sum[i]=0;}//μηδενίζουμε το temporary sum
                double local_prod;
                for(int i =0; i<N/p;i++){//υπολογίζουμε το τοπικό άθροισμα των n/p στοιχείων του επεξεργαστή
                    temp_sum[i]+=local_A[i]*local_B[i];                     //με βάση τον τύπο ΣΑ[i]*b[i]                      
                }
                local_prod=0;
                for(int i=0;i<N/p;i++){//παίρνουμε όλους τους όρους του αθροίσματος και τους 
                    //πολλαπλασιάζουμε για να πάρουμε το γινόμενο όλων των στοιχείων των δύο πινάκων
                    local_prod+=temp_sum[i];
                }
                double final[N],final_prod;
                MPI_Reduce( &local_prod ,&final_prod ,1 ,MPI_DOUBLE , MPI_SUM , root , MPI_COMM_WORLD);
                if(rank==0){
                    printf("The product of all items is %lf",final_prod);
                }


                break;

            }
            case 4: {
                if(rank==0){
                    printf("Please enter N:");
                    fflush(stdout);  
            
                    scanf("%d",&N);
                    while(N!=p){
                        printf("Please enter N:");
                        fflush(stdout);  
            
                        scanf("%d",&N);
                    }

                }  
                MPI_Bcast( &N , 1 , MPI_INT, root , MPI_COMM_WORLD);//Στέλνουμε σε όλους του επεξεργαστές το μέγεθος των πινάκων
                double C[N][N];
                double D[N][N];
                double local_res[N];
                double C_to_be_sent[N];
                double D_to_be_sent[N];
                double local_C[N];
                double local_D[N];
                if(rank==0){
                    
                    for(int i = 0; i<N;i++){
                        for(int j = 0; j<N;j++){
                            printf("Enter C[%d][%d]",i,j);
                            fflush(stdout);  
            
                            scanf("%lf",&C[i][j]);
                            
                        }
                        
                    }
                     for(int i = 0; i<N;i++){
                        for(int j = 0; j<N;j++){
                            printf("Enter D[%d][%d]",i,j);
                            fflush(stdout);  
            
                            scanf("%lf",&D[i][j]);
                            
                        }
                        
                    }
                    
                }
                MPI_Scatter( C , N , MPI_DOUBLE , local_C , N , MPI_DOUBLE , 0 , MPI_COMM_WORLD);//Χωρίζουμε το C σε N ίσα κομμάτια και το στέλνουμε σε όλους τους επεξεργαστές
                //έτσι ώστε ο κάθε επεξεργαστής να πάρει μία γραμμή του πίνακα, με βάση τον αλγόριθμο
                MPI_Scatter( D , N , MPI_DOUBLE , local_D , N , MPI_DOUBLE , 0 , MPI_COMM_WORLD);//Χωρίζουμε το D σε N ίσα κομμάτια και το στέλνουμε σε όλους τους επεξεργαστές
                //έτσι ώστε ο κάθε επεξεργαστής να πάρει μία γραμμή του πίνακα, με βάση τον αλγόριθμο
                for(int i = 0; i<N;i++){
                    local_res[i]=0;
                }    
                for(int i=0;i<N;i++){
                    local_res[i]+=local_C[rank]*local_D[i];//υπολογίζουμε το τοπικό γινόμενο της γραμμής του C και D του κάθε επεξεργαστή. Επειδή κάνουμε scatter 
                    //τον C και τον D σε Ν κομμάτια, θα ισχύει ότι ο local_C[rank] θα είναι η γραμμή του C που αντιστοιχεί στον εκάστοτε επεξεργαστή. 
                }
                int next_proc,prev_proc;
                //υπολογίζουμε ποιος θα είναι ο επόμενος και ο προηγούμενος επεξεργαστής έτσι ώστε να μπορούμε να στείλουμε/λάβουμε την γραμμή του D που 
                //απαιτείται για τον υπολογισμό του αποτελέσματος με βάση τον αλγόριθμο
                if(rank==0){
                        next_proc = rank+1; //αν είναι ο P0 ο επόμενος επεξεργαστής θα είναι ο P0+1=P1
                        prev_proc = p-1;//αν είναι ο P0 ο προηγούμενος επεξεργαστής θα είναι ο P-1=P3 (έστω ότι P=4) διότι οι επεξεργαστές είναι 
                        //συνδεδεμένοι σε δακτύλιο άρα ο P0 και ο Pτελικός συνδέονται μεταξύ τους
                    }else if(rank==p-1){
                        next_proc=0;//αν ο P είναι ο τελευταίος στη σειρά του δακτυλίου, τότε ο επόμενος επεξεργαστής θα είναι ο P0
                        prev_proc=rank-1;//αν ο P είναι ο τελευταίος στη σειρά του δακτυλίου, τότε ο προηγούμενος επεξεργαστής θα είναι ο Pτελικός-1

                    }else{//αλλιώς, ο επόμενος και προηγούμενος επεξεργαστής δίνεται απλώς με μία πρόσθεση ή αφαίρεση με το 1
                        next_proc=rank+1;
                        prev_proc=rank-1;
                    }

                
                for(int k = 1;k<p;k++ ){ //για όλους τους επεξεργαστές εκτός του 0, αφού έχουμε υπολογίσει το τοπικό γινόμενο για τον 0
                    
                    double received_D[N];
                    MPI_Send(local_D,N,MPI_DOUBLE,prev_proc,0,MPI_COMM_WORLD);//στέλνουμε την γραμμή D που έχουμε στον προηγούμενο επεξεργαστή
                    MPI_Recv( received_D , N , MPI_DOUBLE , next_proc , 0,MPI_COMM_WORLD , MPI_STATUS_IGNORE);//λαμβάνουμε την γραμμή D από τον επόμενο επεξεργαστή
                    for(int j = 0;j<N;j++){
                        local_D[j]=received_D[j]; //η γραμμή D που είχαμε θα γίνει ίση με αυτή που λάβαμε
                    }

                    int column = (rank + k)%p;// θα υπολογίσουμε την στήλη του C με την οποία θα κάνουμε τις επόμενες πράξεις: 
                    //Έστω rank = 2 k=1 p=4: column = (2+1)mod4 = 3 mod 4= 3, k=2 p=4: column = (2+2)mod4 = 4 mod 4= 0, 
                    //k=3 p=4: column = (2+3)mod4 = 5 mod 4= 1,άρα για κάθε επανάληψη του βρόχου, υπολογίζουμε παρακάτω το τοπικό 
                    //αποτέλεσμα πολλαπλασιάζοντας την γραμμή του D με την ανάλογη στήλη και γραμμή του C

                    for(int l = 0; l<N; l++ ){
                        local_res[l]+=local_C[column]*local_D[l];
                    }
                }

                double final_res[N][N]; 
                MPI_Gather(local_res,N,MPI_DOUBLE,final_res,N,MPI_DOUBLE,0,MPI_COMM_WORLD);//παίρνουμε όλα τα τοπικά
                // αποτελέσματα όλων των επεξεργαστών και τα βάζουμε σε ένα πίνακα τελικού αποτελέσματος

                if(rank==0){
                    printf("Πίνακας Αποτελέσματος: \n");
                    for(int i = 0; i<N;i++){
                        for(int j =0; j<N;j++){
                            printf("R[%d][%d] = %lf ",i,j,final_res[i][j]);
                        }
                        printf("\n");
                    }
                }
                break;

            }
            case 5:{//exit
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

    
  

