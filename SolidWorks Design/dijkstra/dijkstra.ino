#define INF 99999
#define Max 55

char now='U';
String GO="..";

bool MAP1[Max][Max];


int start=1;
int End;
void fill_map();
void dijkstra(int,int);
void setup() {
  Serial.begin(9600);
  fill_map();
}

void loop() {
  Serial.println('.');
  if(Serial.available())
  {
    String x=Serial.readString();
    Serial.println(x);
    char y[2]{x[0],x[1]};
    int n=atoi(y);
    Serial.println(n);
    if(n<=99)
      End=Room[n-1];
    else if(n==1111)
       End=pharmacy;
    else if(n==1212)
       End=food;
    else if(n==1010)
       End=report;
    else
       End=0;
       Serial.println(End);
    if(End!=0)
    {
      dijkstra(start,End);
      start=End;
      Serial.println(GO);
      delay(1000);
    }
  }
}
void fill_map(){
    for (int i=0;i<Max;i++)
    {
        if (i==1||i==2||i==3||i==17||i==18||i==19||i==20)
        {

            MAP1[i][i+11]=1;
        }
        else if (i==12)
        {
            MAP1[i][i+1]=1;
            MAP1[i][i+11]=1;
            MAP1[i][i-11]=1;
        }
        else if (i==13||i==14||i==23||i==24||i==25||i==26||i==28||i==29||i==30||i==31||i==35)
        {
          MAP1[i][i+1]=1;
          MAP1[i][i-1]=1;
            MAP1[i][i+11]=1;
            MAP1[i][i-11]=1;
        }
        else if (i==32)
        {

            MAP1[i][i-1]=1;

        }
        else if (i==15)
        {
            MAP1[i][i-1]=1;
            MAP1[i][i+11]=1;
        }
        else if (i==22)
        {
            MAP1[i][i+1]=1;

        }
        else if (i==39||i==40||i==41||i==42||i==46||i==48)
        {
            MAP1[i][i-11]=1;
        }
        else if(i==37)
        {
            MAP1[i][i-1]=1;
            MAP1[i][i-11]=1;
            MAP1[i][i+11]=1;
        }
        else if(i==36)
        {
            MAP1[i][i-1]=1;
            MAP1[i][i+1]=1;
            MAP1[i][i-11]=1;
        }
        else if(i==34)
           {
               MAP1[i][i+1]=1;
               MAP1[i][i-11]=1;
           }
           else if(i==27)
           {
                MAP1[i][i-1]=1;
                 MAP1[i][i+1]=1;
           }
    }
}
void dijkstra( int startnode, int Goal){
    int n=Max; // size of map .
    int cost[Max][Max] ; // cost is the map with one diffirence .
     int distance[Max]; // the distance from start to end .
     int pred[Max];  // store the path nodes .
    bool visited[Max]; // set the node if the code visited it or not .
    int count,nextnode = 0,i,j;
    int mindistance;

    for(i=0;i<n;i++)
        for(j=0;j<n;j++)
            if(MAP1[i][j]==0)
                cost[i][j]=INF; // pass over the map and set the ZERO length as INF .
            else
                cost[i][j]=MAP1[i][j];
    for(i=0;i<n;i++)
    {
        distance[i]=cost[startnode][i];
        pred[i]=startnode;
        visited[i]=0;
    }
    distance[startnode]=0;
    visited[startnode]=1;
    count=1;
    while(count<n-1)
    {
        mindistance=INF;
        for(i=0;i<n;i++)
            if(distance[i]<mindistance&&!visited[i])
            {
                mindistance=distance[i];
                nextnode=i;
            }
        visited[nextnode]=1;
        for(i=0;i<n;i++)
            if(!visited[i])
                if(mindistance+cost[nextnode][i]<distance[i])
                {
                    distance[i]=mindistance+cost[nextnode][i];
                    pred[i]=nextnode;
                }
        count++;
    }
    int PathZise=0; // store the size of path array .
    for(i=0;i<n;i++) // first loop to calculate the size of path array .
        if(i!=startnode && i==Goal)
        {
            j=i;
            do
            {
                j=pred[j];
                PathZise++;
            }while(j!=startnode);
        }
    int path[PathZise+1]; // path array to store the path nodes .
    PathZise=0;
    for(i=0;i<n;i++) // second loop to store the path nodes .
        if(i!=startnode && i==Goal)
        {
            j=i;
            do
            {
                j=pred[j];
                path[PathZise]=j;
                PathZise++;
            }while(j!=startnode);
        }
    int path2[PathZise]; // new path array to store the first .
    for (int i=0;i<PathZise;i++) // store the same path .
        path2[i]=path[i];

    for (i=0;i<PathZise;i++) // inverse the path .
        path[PathZise-1-i]=path2[i];
    path[PathZise]=Goal; // final node is the Goal .

    i=0;
    while(i<PathZise) // loop to build the path string .
    {
            if  ( now=='U' && path[i+1]==path[i]+1 )
            {
                GO[i]='R';now='R';
            }
            else if (now=='U'&&path[i+1]==path[i]-1)
            {
                GO[i]='L';now='L';
            }
            else if (now=='U'&&path[i+1]==path[i]+11)
            {                                         // current situation of the robot
                GO[i]='F';now='U';                    //            U
            }                                         //         R     L
            else if (now=='D'&&path[i+1]==path[i]-1)  //            D
            {
                GO[i]='R';now='L';
            }
            else if (now=='D'&&path[i+1]==path[i]+1)  // F : Forward
            {                                         // W : Wide Turn
                GO[i]='L';now='R';                    // L : Left
            }                                         // R : Right
            else if (now=='D'&&path[i+1]==path[i]-11)
            {
                GO[i]='F';now='D';
            }
            else if (now=='R'&&path[i+1]==path[i]+1)
            {
                GO[i]='F';now='R';
            }
            else if (now=='R'&&path[i+1]==path[i]-11)
            {
                GO[i]='R';now='D';
            }
            else if (now=='R'&&path[i+1]==path[i]+11)
            {
                GO[i]='L';now='U';
            }
            else if (now=='L'&&path[i+1]==path[i]-1)
            {
                GO[i]='F';now='L';
            }
            else if (now=='L'&&path[i+1]==path[i]+11)
            {
                GO[i]='R';now='U';
            }
            else if (now=='L'&&path[i+1]==path[i]-11)
            {
                GO[i]='L';now='D';
            }
            else if (now=='U'&&path[i+1]==path[i]-11)
            {
                GO[i]='W';now='D';
            }
            else if (now=='D'&&path[i+1]==path[i]+11)
            {
                GO[i]='W';now='U';
            }
            else if (now=='R'&&path[i+1]==path[i]-1)
            {
                GO[i]='W';now='L';
            }
            else if (now=='L'&&path[i+1]==path[i]+1)
            {
                GO[i]='W';now='R';
            }
            i++;
    }
    GO[i]='.';  // '.' : to stop on final node .
}
