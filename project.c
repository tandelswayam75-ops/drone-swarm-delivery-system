#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_DRONES 10
#define MAX_TASKS 50
#define MAX_LOCATIONS 6
#define INF 9999.0 
struct Drone
{
    int id;
    int battery;
    int status;
    int location;
}; 
struct Task
{
    int taskID;
    char source[30];
    char destination[30];
    int sourceLocation;
    int destinationLocation;
    int priority;
    float distance;
    int droneID;
};
struct History
{
    int taskID;
    int droneID;
    char destination[30];
    float distance;
    struct History *next;
};
struct Drone drones[MAX_DRONES];
int droneCount = 5; 
struct Task emergencyQueue[MAX_TASKS];
int emergencyFront = -1;
int emergencyRear = -1;
int emergencyCount = 0; 
struct Task highQueue[MAX_TASKS];
int highFront = -1;
int highRear = -1;
int highCount = 0; 
struct Task normalQueue[MAX_TASKS];
int normalFront = -1;
int normalRear = -1;
int normalCount = 0;                
struct History *historyHead = NULL; 
float graph[MAX_LOCATIONS][MAX_LOCATIONS];
char locations[MAX_LOCATIONS][30] = {"Main Gate", "Girl Hostel", "College Campus", "Boys Hostel", "Sports Ground", "Staff Quarter"};
int completedDeliveries = 0;
float totalDistance = 0; 
void initializeDrones()
{
    drones[0].id = 101;
    drones[0].battery = 100;
    drones[0].status = 0;
    drones[0].location = 0;
    drones[1].id = 102;
    drones[1].battery = 100;
    drones[1].status = 0;
    drones[1].location = 1;
    drones[2].id = 103;
    drones[2].battery = 100;
    drones[2].status = 0;
    drones[2].location = 2;
    drones[3].id = 104;
    drones[3].battery = 100;
    drones[3].status = 0;
    drones[3].location = 4;
    drones[4].id = 105;
    drones[4].battery = 100;
    drones[4].status = 0;
    drones[4].location = 5;
    drones[5].id = 106;
    drones[5].battery = 100;
    drones[5].status = 0;
    drones[5].location = 5;
}
void displayDrones()
{
    int i;

    printf("\n================ DRONE DETAILS ================\n");
    printf("%-10s %-10s %-15s %-20s\n",
           "Drone ID", "Battery", "Status", "Location");
    printf("------------------------------------------------------------\n");
    for (i = 0; i < droneCount; i++)
    {
        printf("%-10d %-10d ",drones[i].id,drones[i].battery);
        if (drones[i].status == 0)
            printf("%-15s", "Available");
        else if (drones[i].status == 1)
            printf("%-15s", "Busy");
        else
            printf("%-15s", "Charging");
        printf("%-20s\n", locations[drones[i].location]);
    }
}
void enqueueEmergency(struct Task task)
{
    if (emergencyFront == 0 && emergencyRear == MAX_TASKS - 1)
    {
        printf("\nEmergency queue is full.\n");
        return;
    }

    if (emergencyFront == -1 && emergencyRear == -1)
    {
        emergencyFront = 0;
        emergencyRear = 0;
    }
    else if (emergencyRear == MAX_TASKS - 1 && emergencyFront != 0)
    {
        emergencyRear = 0;
    }
    else
    {
        emergencyRear = emergencyRear + 1;
    }

    emergencyQueue[emergencyRear] = task;
    emergencyCount++;
}
void enqueueHigh(struct Task task)
{
    if (highFront == 0 && highRear == MAX_TASKS - 1)
    {
        printf("\nHigh priority queue is full.\n");
        return;
    }

    if (highFront == -1 && highRear == -1)
    {
        highFront = 0;
        highRear = 0;
    }
    else if (highRear == MAX_TASKS - 1 && highFront != 0)
    {
        highRear = 0;
    }
    else
    {
        highRear = highRear + 1;
    }

    highQueue[highRear] = task;
    highCount++;
}

struct Task dequeueHigh()
{
    struct Task task;
    if (highFront == -1)
    {
        printf("\nHigh priority queue is empty.\n");
        return task;
    }
    task = highQueue[highFront];
    if (highFront == highRear)
    {
        highFront = -1;
        highRear = -1;
    }
    else if (highFront == MAX_TASKS - 1)
    {
        highFront = 0;
    }
    else
    {
        highFront = highFront + 1;
    }
    highCount--;
    return task;
}
void enqueueNormal(struct Task task)
{
    if (normalFront == 0 && normalRear == MAX_TASKS - 1)
    {
        printf("\nNormal priority queue is full.\n");
        return;
    }

    if (normalFront == -1 && normalRear == -1)
    {
        normalFront = 0;
        normalRear = 0;
    }
    else if (normalRear == MAX_TASKS - 1 && normalFront != 0)
    {
        normalRear = 0;
    }
    else
    {
        normalRear = normalRear + 1;
    }
    normalQueue[normalRear] = task;
    normalCount++;
}

struct Task dequeueNormal()
{
    struct Task task;
    if (normalFront == -1)
    {
        printf("\nNormal priority queue is empty.\n");
        return task;
    }
    task = normalQueue[normalFront];
    if (normalFront == normalRear)
    {
        normalFront = -1;
        normalRear = -1;
    }
    else if (normalFront == MAX_TASKS - 1)
    {
        normalFront = 0;
    }
    else
    {
        normalFront = normalFront + 1;
    }
    normalCount--;
    return task;
}
struct Task dequeueEmergency()
{
    struct Task task;
    if (emergencyFront == -1)
    {
        printf("\nEmergency queue is empty.\n");
        return task;
    }
    task = emergencyQueue[emergencyFront];
    if (emergencyFront == emergencyRear)
    {
        emergencyFront = -1;
        emergencyRear = -1;
    }
    else if (emergencyFront == MAX_TASKS - 1)
    {
        emergencyFront = 0;
    }
    else
    {
        emergencyFront = emergencyFront + 1;
    }
    emergencyCount--;
    return task;
}
float calculateShortestDistance(int source, int destination)
{
    float distance[MAX_LOCATIONS];
    int visited[MAX_LOCATIONS];
    int i, j;
    float minDistance;
    int current;
    for (i = 0; i < MAX_LOCATIONS; i++)
    {
        distance[i] = INF;
        visited[i] = 0;
    }
    distance[source] = 0;
    for (i = 0; i < MAX_LOCATIONS; i++)
    {
        minDistance = INF;
        current = -1;
        for (j = 0; j < MAX_LOCATIONS; j++)
        {
            if (!visited[j] && distance[j] < minDistance)
            {
                minDistance = distance[j];
                current = j;
            }
        }
        if (current == -1)
        {
            break;
        }
        visited[current] = 1;
        for (j = 0; j < MAX_LOCATIONS; j++)
        {
            if (graph[current][j] != INF && !visited[j] && distance[current] + graph[current][j] < distance[j])
            {
                distance[j] = distance[current] + graph[current][j];
            }
        }
    }
    return distance[destination];
} 
void addTask()
{
    struct Task newTask;
    int i;
    printf("\nEnter Task ID: ");
    scanf("%d", &newTask.taskID);
    printf("\nAvailable Locations:\n");
    for (i = 0; i < MAX_LOCATIONS; i++)
    {
        printf("%d. %s\n", i, locations[i]);
    }
    printf("\nEnter Source Location Number: ");
    scanf("%d", &newTask.sourceLocation);
    printf("Enter Destination Location Number: ");
    scanf("%d", &newTask.destinationLocation);
    if (newTask.sourceLocation < 0 || newTask.sourceLocation >= MAX_LOCATIONS || newTask.destinationLocation < 0 || newTask.destinationLocation >= MAX_LOCATIONS)
    {
        printf("\nInvalid location.\n");
        return;
    }
    strcpy(newTask.source, locations[newTask.sourceLocation]);
    strcpy(newTask.destination, locations[newTask.destinationLocation]);
    printf("\nEnter Priority:\n");
    printf("1. Emergency\n");
    printf("2. High\n");
    printf("3. Normal\n");
    printf("Enter priority: ");
    scanf("%d", &newTask.priority);
    if (newTask.priority < 1 || newTask.priority > 3)
    {
        printf("\nInvalid priority.\n");
        return;
    } 
    newTask.distance = calculateShortestDistance(newTask.sourceLocation, newTask.destinationLocation);
    newTask.droneID = -1; 
    if (newTask.priority == 1)
    {
        enqueueEmergency(newTask);
    }
    else if (newTask.priority == 2)
    {
        enqueueHigh(newTask);
    }
    else
    {
        enqueueNormal(newTask);
    }
    printf("\nTask added successfully.\n");
    printf("Task ID: %d\n", newTask.taskID);
    printf("Shortest Task Distance: %.1f km\n", newTask.distance);
} 
int getTotalTasks()
{
    return emergencyCount + highCount + normalCount;
} 
void displayOneTask(struct Task task)
{
    printf("%-10d %-20s %-20s ", task.taskID, task.source, task.destination);
    if (task.priority == 1)
    {
        printf("%-12s", "Emergency");
    }
    else if (task.priority == 2)
    {
        printf("%-12s", "High");
    }
    else
    {
        printf("%-12s", "Normal");
    }
    printf("%.1f km\n", task.distance);
} 
void displayTasks()
{
    int i;
    int index;
    if (getTotalTasks() == 0)
    {
        printf("\nNo pending tasks.\n");
        return;
    }
    printf("\n================ PENDING TASKS ================\n");
    printf("%-10s %-20s %-20s %-12s %-10s\n", "Task ID", "Source", "Destination", "Priority", "Distance");
    printf("--------------------------------------------------------------------------\n"); /* Emergency */
    if (emergencyCount > 0)
    {
        index = emergencyFront;
        for (i = 0; i < emergencyCount; i++)
        {
            displayOneTask(emergencyQueue[index]);
            index = (index + 1) % MAX_TASKS;
        }
    }
    if (highCount > 0)
    {
        index = highFront;
        for (i = 0; i < highCount; i++)
        {
            displayOneTask(highQueue[index]);
            index = (index + 1) % MAX_TASKS;
        }
    }
    if (normalCount > 0)
    {
        index = normalFront;
        for (i = 0; i < normalCount; i++)
        {
            displayOneTask(normalQueue[index]);
            index = (index + 1) % MAX_TASKS;
        }
    }
} 
struct Task getNextTask()
{
    struct Task task;
    if (emergencyCount > 0)
    {
        task = dequeueEmergency();
    }
    else if (highCount > 0)
    {
        task = dequeueHigh();
    }
    else
    {
        task = dequeueNormal();
    }
    return task;
} 
void addHistory(int taskID, int droneID, char destination[], float distance)
{
    struct History *newNode;
    newNode = (struct History *)malloc(sizeof(struct History));
    if (newNode == NULL)
    {
        printf("\nMemory allocation failed.\n");
        return;
    }
    newNode->taskID = taskID;
    newNode->droneID = droneID;
    strcpy(newNode->destination, destination);
    newNode->distance = distance;
    newNode->next = historyHead;
    historyHead = newNode;
} 
void assignDrone()
{
    struct Task task;
    int i;
    int selected = -1;
    float minimumDistance = INF;
    float droneToSource;
    float sourceToDestination;
    float returnDistance;
    float requiredBattery;
    float totalFlightDistance;
    if (getTotalTasks() == 0)
    {
        printf("\nNo pending tasks available.\n");
        return;
    }
    task = getNextTask();
    printf("\n================ TASK SELECTED ================\n");
    printf("Task ID       : %d\n", task.taskID);
    printf("Source        : %s\n", task.source);
    printf("Destination   : %s\n", task.destination);
    printf("Task Distance : %.1f km\n", task.distance);
    printf("Priority      : ");
    if (task.priority == 1)
    {
        printf("Emergency\n");
    }
    else if (task.priority == 2)
    {
        printf("High\n");
    }
    else
    {
        printf("Normal\n");
    } 
    for (i = 0; i < droneCount; i++)
    {
        if (drones[i].status == 0)
        {                                                                                                   /* Drone to source */
            droneToSource = calculateShortestDistance(drones[i].location, task.sourceLocation);             /* Source to destination */
            sourceToDestination = calculateShortestDistance(task.sourceLocation, task.destinationLocation); /* Destination back to drone */
            returnDistance = calculateShortestDistance(task.destinationLocation, drones[i].location);       /* Total flight distance */
            totalFlightDistance = droneToSource + sourceToDestination + returnDistance;                     /* 10% battery for every km */
            requiredBattery = totalFlightDistance * 10;                                                     /* Check battery */
            if (drones[i].battery >= requiredBattery)
            {
                if (droneToSource < minimumDistance)
                {
                    minimumDistance = droneToSource;
                    selected = i;
                }
            }
        }
    } 
    if (selected == -1)
    {
        printf("\nNo suitable drone is currently available.\n"); 
        if (task.priority == 1)
        {
            enqueueEmergency(task);
        }
        else if (task.priority == 2)
        {
            enqueueHigh(task);
        }
        else
        {
            enqueueNormal(task);
        }
        return;
    } 
    drones[selected].status = 1;
    task.droneID = drones[selected].id;
    printf("\nDrone Assigned Successfully!\n");
    printf("Task ID        : %d\n", task.taskID);
    printf("Drone ID       : %d\n", drones[selected].id);
    printf("Drone Battery  : %d%%\n", drones[selected].battery);
    printf("Drone to Source: %.1f km\n", minimumDistance);
    printf("Required Battery: %.1f%%\n", requiredBattery); 
    drones[selected].location = task.destinationLocation; 
    drones[selected].battery -= (int)requiredBattery;
    if (drones[selected].battery < 0)
    {
        drones[selected].battery = 0;
    } /* Store completed task */
    addHistory(task.taskID, drones[selected].id, task.destination, sourceToDestination); /* Make drone available again */
    drones[selected].status = 0;
    completedDeliveries++;
    totalDistance += sourceToDestination;
    printf("\nDelivery completed automatically.\n");
    printf("Task ID    : %d\n", task.taskID);
    printf("Drone ID   : %d\n", drones[selected].id);
    printf("Destination: %s\n", task.destination);
    printf("Distance   : %.1f km\n", sourceToDestination);
    printf("Remaining Battery: %d%%\n", drones[selected].battery);
} 
void displayHistory()
{
    struct History *temp;
    if (historyHead == NULL)
    {
        printf("\nNo completed deliveries.\n");
        return;
    }
    temp = historyHead;
    printf("\n================ DELIVERY HISTORY ================\n");
    printf("%-10s %-10s %-25s %-10s\n", "Task ID", "Drone ID", "Destination", "Distance");
    printf("-------------------------------------------------------------\n");
    while (temp != NULL)
    {
        printf("%-10d %-10d %-25s %-10.1f\n", temp->taskID, temp->droneID, temp->destination, temp->distance);
        temp = temp->next;
    }
} 
void initializeGraph()
{
    int i;
    int j; /* Initially no connection */
    for (i = 0; i < MAX_LOCATIONS; i++)
    {
        for (j = 0; j < MAX_LOCATIONS; j++)
        {
            if (i == j)
            {
                graph[i][j] = 0;
            }
            else
            {
                graph[i][j] = INF;
            }
        }
    }
    graph[0][1] = 0.7; /* Main Gate <-> Girl Hostel */
    graph[1][0] = 0.7;

    graph[1][5] = 0.1; /* Girl Hostel <-> Staff Quarter */
    graph[5][1] = 0.1; 

    graph[1][3] = 0.2; /* Girl Hostel <-> Boys Hostel */
    graph[3][1] = 0.2; 

    graph[2][4] = 0.1; /* College Campus <-> Sports Ground */
    graph[4][2] = 0.1;

    graph[4][3] = 0.1; /* Sports Ground <-> Boys Hostel */
    graph[3][4] = 0.1; 
    
    graph[0][2] = 0.5;
    graph[2][0] = 0.5; /* Main Gate <-> College Campus */

    graph[2][3] = 0.5;/* College Campus <-> Boys Hostel */
    graph[3][2] = 0.5;

    graph[0][3] = 0.6; /* Main Gate <-> Boys Hostel */
    graph[3][0] = 0.6;
}
void dijkstra(int source, int destination)
{
    float distance[MAX_LOCATIONS];
    int visited[MAX_LOCATIONS];
    int parent[MAX_LOCATIONS];
    int path[MAX_LOCATIONS];
    int pathCount = 0;
    int i;
    int j;
    float minDistance;
    int current; /* Initialize */
    for (i = 0; i < MAX_LOCATIONS; i++)
    {
        distance[i] = INF;
        visited[i] = 0;
        parent[i] = -1;
    }
    distance[source] = 0;
    for (i = 0; i < MAX_LOCATIONS; i++)
    {
        minDistance = INF;
        current = -1;
        for (j = 0; j < MAX_LOCATIONS; j++)
        {
            if (!visited[j] && distance[j] < minDistance)
            {
                minDistance = distance[j];
                current = j;
            }
        }
        if (current == -1)
        {
            break;
        }
        visited[current] = 1;
        for (j = 0; j < MAX_LOCATIONS; j++)
        {
            if (graph[current][j] != INF && !visited[j] && distance[current] + graph[current][j] < distance[j])
            {
                distance[j] = distance[current] + graph[current][j];
                parent[j] = current;
            }
        }
    } 
    if (distance[destination] == INF)
    {
        printf("\nNo route available.\n");
        return;
    } /* Store path */
    current = destination;
    while (current != -1)
    {
        path[pathCount] = current;
        pathCount++;
        current = parent[current];
    }
    printf("\nShortest Route:\n");
    for (i = pathCount - 1; i >= 0; i--)
    {
        printf("%s", locations[path[i]]);
        if (i != 0)
        {
            printf(" -> ");
        }
    }
    printf("\nTotal Distance = %.1f km\n", distance[destination]);
} 
void findShortestRoute()
{
    int source;
    int destination;
    int i;
    printf("\nAvailable Locations:\n");
    for (i = 0; i < MAX_LOCATIONS; i++)
    {
        printf("%d. %s\n", i, locations[i]);
    }
    printf("\nEnter Source Location: ");
    scanf("%d", &source);
    printf("Enter Destination Location: ");
    scanf("%d", &destination);
    if (source < 0 || source >= MAX_LOCATIONS || destination < 0 || destination >= MAX_LOCATIONS)
    {
        printf("\nInvalid location.\n");
        return;
    }
    dijkstra(source, destination);
} 
void displayMap()
{
    printf("\n================ CAMPUS MAP ================\n");
    printf("\nMain Gate -------------|\n");
    printf("    |                    |            \n");
    printf("    |                    ----0.7km--------------girls Hostel--0.1km--Staff quater\n");
    printf("    |     College Campus                              | \n");
    printf("    |     |      |                                   0.2km \n");
    printf("    |     |      |-0.1km--Sport Ground--0.1km------- boys hostel                                    | \n");
    printf("    |     0.2                                         |  \n");
    printf("    0.3km |                                           |\n");
    printf("    |     |                                           |\n");
    printf("    ------|----------------0.3km----------------------|\n");
}
void displayStatistics()    
{
    int i;
    int available = 0;
    int busy = 0;
    int charging = 0;
    for (i = 0; i < droneCount; i++)
    {
        if (drones[i].status == 0)
        {
            available++;
        }
        else if (drones[i].status == 1)
        {
            busy++;
        }
        else
        {
            charging++;
        }
    }
    printf("\n================ SYSTEM STATISTICS ================\n");
    printf("Total Drones           : %d\n", droneCount);
    printf("Available Drones       : %d\n", available);
    printf("Busy Drones            : %d\n", busy);
    printf("Charging Drones        : %d\n", charging);
    printf("Pending Tasks          : %d\n", getTotalTasks());
    printf("Completed Deliveries   : %d\n", completedDeliveries);
    printf("Total Delivery Distance: %.1f km\n", totalDistance);
}
int main()
{
    int choice;
    initializeDrones();
    initializeGraph();
    do
    {
        printf("\n\n================================================\n");
        printf("        DRONE SWARM DELIVERY SYSTEM\n");
        printf("================================================\n");
        printf("1. Display All Drones\n");
        printf("2. Add Delivery Task\n");
        printf("3. Display Pending Tasks\n");
        printf("4. Assign Drone to Task\n");
        printf("5. Find Shortest Route\n");
        printf("6. Display Delivery History\n");
        printf("7. Display Campus Map\n");
        printf("8. Display System Statistics\n");
        printf("9. Exit\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            displayDrones();
            break;
        case 2:
            addTask();
            break;
        case 3:
            displayTasks();
            break;
        case 4:
            assignDrone();
            break;
        case 5:
            findShortestRoute();
            break;
        case 6:
            displayHistory();
            break;
        case 7:
            displayMap();
            break;
        case 8:
            displayStatistics();
            break;
        case 9:
            printf("\nThank you for using the system.\n");
            break;
        default:
            printf("\nInvalid choice. Please try again.\n");
        }
    } while (choice != 9);
    return 0;
}