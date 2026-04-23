#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

#define MAX 25
#define INF 999999

char places[MAX][50] = {
    "Main Gate","Admission Office","Volleyball Court","Green Garden",
    "Mosque","Transport","Boys Hostel 1","Academic Building 1",
    "Academic Building 3","Academic Building 4","Golf Court",
    "Swimming Pool","Kathal Tola","Auditorium","Girls Hostel",
    "Food Court","Bonomaya","Cafeteria","Zoo Zone","Lake","Boys Hostel 2"
};

int graph[MAX][MAX];
int totalPlaces = 21;


void line(){ for(int i=0;i<90;i++) printf("="); printf("\n"); }
void dash(){ for(int i=0;i<60;i++) printf("-"); printf("\n"); }
void star(){ for(int i=0;i<90;i++) printf("*"); printf("\n"); }

void center(char text[]){
    int len=strlen(text);
    int sp=(90-len)/2;
    for(int i=0;i<sp;i++) printf(" ");
    printf("%s\n",text);
}

void header(char title[]){
    line();
    center("CAMPUS NAVIGATION SYSTEM");
    center(title);
    line();
}

void pause(){
    printf("\nPress Enter to continue...");
    getchar(); getchar();
}



void initGraph(){

    for(int i=0;i<MAX;i++)
        for(int j=0;j<MAX;j++)
            graph[i][j]=0;

    graph[0][1]=50; graph[1][0]=50;
    graph[0][10]=70; graph[10][0]=70;

    graph[10][11]=40; graph[11][10]=40;
    graph[11][12]=30; graph[12][11]=30;
    graph[12][9]=60; graph[9][12]=60;

    graph[9][13]=50; graph[13][9]=50;
    graph[13][14]=80; graph[14][13]=80;
    graph[13][15]=90; graph[15][13]=90;

    graph[15][17]=20; graph[17][15]=20;
    graph[17][18]=25; graph[18][17]=25;
    graph[18][20]=30; graph[20][18]=30;
    graph[20][19]=40; graph[19][20]=40;

    graph[18][16]=60; graph[16][18]=60;

    graph[15][8]=70; graph[8][15]=70;
    graph[8][5]=50; graph[5][8]=50;
    graph[5][6]=30; graph[6][5]=30;

    graph[8][4]=60; graph[4][8]=60;
    graph[4][2]=40; graph[2][4]=40;
    graph[2][1]=30; graph[1][2]=30;

    graph[4][7]=35; graph[7][4]=35;
    graph[7][3]=45; graph[3][7]=45;
}



void showPlaces(){
    header("ALL PLACES");
    for(int i=0;i<totalPlaces;i++)
        printf("[%2d] %-30s\n", i, places[i]);
    line();
}

void showConnections(){
    header("ALL CONNECTIONS");

    int count=0;

    for(int i=0;i<totalPlaces;i++){
        for(int j=i+1;j<totalPlaces;j++){
            if(graph[i][j]){
                printf("%-25s <--> %-25s : %3d m\n",
                places[i],places[j],graph[i][j]);
                count++;
            }
        }
    }

    dash();
    printf("Total Roads: %d\n",count);
    line();
}


int minNode(int dist[], int vis[]){
    int min=INF, idx=-1;
    for(int i=0;i<totalPlaces;i++){
        if(!vis[i] && dist[i]<min){
            min=dist[i];
            idx=i;
        }
    }
    return idx;
}

void printForward(int parent[], int j){
    if(parent[j]==-1){
        printf("%s",places[j]);
        return;
    }
    printForward(parent,parent[j]);
    printf(" -> %s",places[j]);
}

void printReverse(int parent[], int j){
    while(j!=-1){
        printf("%s <- ",places[j]);
        j=parent[j];
    }
    printf("START\n");
}

void printStepDistance(int parent[], int dest){
    int path[MAX], count=0;

    while(dest!=-1){
        path[count++]=dest;
        dest=parent[dest];
    }

    for(int i=count-1;i>0;i--){
        int u=path[i], v=path[i-1];
        printf("%s -> %s = %d m\n",
        places[u],places[v],graph[u][v]);
    }
}


void dijkstra(int src,int dest){

    int dist[MAX], vis[MAX]={0}, parent[MAX];

    for(int i=0;i<totalPlaces;i++){
        dist[i]=INF;
        parent[i]=-1;
    }

    dist[src]=0;

    for(int i=0;i<totalPlaces;i++){
        int u=minNode(dist,vis);
        if(u==-1) break;

        vis[u]=1;

        for(int v=0;v<totalPlaces;v++){
            if(graph[u][v] && !vis[v] &&
               dist[u]+graph[u][v] < dist[v]){
                dist[v]=dist[u]+graph[u][v];
                parent[v]=u;
            }
        }
    }

    header("SHORTEST PATH RESULT");

    if(dist[dest]==INF){
        printf("No path found!\n");
        return;
    }

    printf("Distance : %d meters\n",dist[dest]);

    printf("\nForward Path:\n");
    printForward(parent,dest);

    printf("\n\nReverse Path:\n");
    printReverse(parent,dest);

    printf("\nStep Breakdown:\n");
    printStepDistance(parent,dest);

    printf("\nEstimated Time: %.2f min\n",
           dist[dest]/80.0);

    line();
}



void nearest(int src){
    header("NEAREST PLACE");

    int min=INF, idx=-1;

    for(int i=0;i<totalPlaces;i++){
        if(graph[src][i] && graph[src][i]<min){
            min=graph[src][i];
            idx=i;
        }
    }

    if(idx==-1) printf("No nearby place.\n");
    else printf("Nearest: %s (%d m)\n",places[idx],min);

    line();
}

void directRoutes(int src){
    header("DIRECT ROUTES");

    for(int i=0;i<totalPlaces;i++){
        if(graph[src][i]){
            printf("%s -> %s (%d m)\n",
                   places[src],places[i],graph[src][i]);
        }
    }

    line();
}


void mostConnected(){
    header("MOST CONNECTED PLACE");

    int max=0, idx=-1;

    for(int i=0;i<totalPlaces;i++){
        int c=0;
        for(int j=0;j<totalPlaces;j++)
            if(graph[i][j]) c++;

        if(c>max){ max=c; idx=i; }
    }

    printf("%s (%d connections)\n",places[idx],max);

    line();
}


void systemInfo(){
    header("SYSTEM INFO");

    int edges=0;
    for(int i=0;i<totalPlaces;i++)
        for(int j=i+1;j<totalPlaces;j++)
            if(graph[i][j]) edges++;

    printf("Places : %d\n",totalPlaces);
    printf("Roads  : %d\n",edges);
    printf("Algo   : Dijkstra\n");

    line();
}



void menu(){
    line();
    printf("1. Show All Places\n");
    printf("2. Show All Connections\n");
    printf("3. Shortest Path\n");
    printf("4. Suggest Nearest Place\n");
    printf("5. Show Direct Routes\n");
    printf("6. Exit\n");
    line();
    printf("Choice: ");
}



int main(){

    int ch,s,d;

    initGraph();

    while(1){

        header("MAIN MENU");
        menu();

        scanf("%d",&ch);

        switch(ch){

        case 1:
            showPlaces();
            break;

        case 2:
            showConnections();
            break;

        case 3:
            showPlaces();
            printf("Source: "); scanf("%d",&s);
            printf("Destination: "); scanf("%d",&d);
            dijkstra(s,d);
            break;

        case 4:
            showPlaces();
            printf("Source: "); scanf("%d",&s);
            nearest(s);
            break;

        case 5:
            showPlaces();
            printf("Source: "); scanf("%d",&s);
            directRoutes(s);
            break;

        case 6:
            header("EXIT");
            printf("Program Ended\n");
            return 0;

        default:
            printf("Invalid choice\n");
        }

        mostConnected();
        systemInfo();

        pause();
    }
}

