
//************************************************************
// this is a simple example that uses the easyMesh library
//
// 1. blinks led once for every node on the mesh
// 2. blink cycle repeats every BLINK_PERIOD
// 3. sends a silly message to every node on the mesh at a random time between 1 and 5 seconds
// 4. prints anything it receives to Serial.print
//
//
//************************************************************
#include <painlessMesh.h>
#include "topology.h"

// some gpio pin that is connected to an LED...
// on my rig, this is 5, change to the right number of your LED.
#ifdef LED_BUILTIN
#define LED LED_BUILTIN
#else
#define LED 2
#endif
#define BLINK_PERIOD 3000  // milliseconds until cycle repeat
#define BLINK_DURATION 100 // milliseconds LED is on for
#define MESH_SSID "testmeshnetwork"
#define MESH_PASSWORD "testpassword"
#define MESH_PORT 5555
// Prototypes
void sendMessage();
void receivedCallback(uint32_t from, String &msg);
void newConnectionCallback(uint32_t nodeId);
void changedConnectionCallback();
void nodeTimeAdjustedCallback(int32_t offset);
void delayReceivedCallback(uint32_t from, int32_t delay);
void logDetailMesh();
void printNodeTree(const painlessmesh::protocol::NodeTree &node, const String &prefix);
void checkSerialInput();

Scheduler userScheduler; // to control your personal task
painlessMesh mesh;
bool calc_delay = false;
SimpleList<uint32_t> nodes;
void sendMessage();                                                // Prototype
Task taskSendMessage(TASK_SECOND * 1, TASK_FOREVER, &sendMessage); // start with a one second interval
// Task to blink the number of nodes
Task blinkNoNodes;
bool onFlag = false;
void setup()
{
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  mesh.setDebugMsgTypes(ERROR | MESH_STATUS | CONNECTION | SYNC | GENERAL | MSG_TYPES | REMOTE); // set before init() so that you can see error messages
  mesh.setRoot(true);
  mesh.setContainsRoot(true);
  // mesh.init(MESH_SSID, MESH_PASSWORD, MESH_PORT);
  mesh.init(MESH_SSID, MESH_PASSWORD, &userScheduler, MESH_PORT);
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
  mesh.onNodeDelayReceived(&delayReceivedCallback);
  userScheduler.addTask(taskSendMessage);
  taskSendMessage.enable();
  blinkNoNodes.set(BLINK_PERIOD, (mesh.getNodeList().size() + 1) * 2, []()
                   {
      // If on, switch off, else switch on
      if (onFlag)
        onFlag = false;
      else
        onFlag = true;
      blinkNoNodes.delay(BLINK_DURATION);

      if (blinkNoNodes.isLastIteration()) {
        // Finished blinking. Reset task for next run 
        // blink number of nodes (including this node) times
        blinkNoNodes.setIterations((mesh.getNodeList().size() + 1) * 2);
        // Calculate delay based on current mesh time and BLINK_PERIOD
        // This results in blinks between nodes being synced
        blinkNoNodes.enableDelayed(BLINK_PERIOD - 
            (mesh.getNodeTime() % (BLINK_PERIOD*1000))/1000);
      } });
  userScheduler.addTask(blinkNoNodes);
  blinkNoNodes.enable();

  randomSeed(analogRead(A0));
}

void loop()
{
  mesh.update();
  digitalWrite(LED, !onFlag);
  checkSerialInput();
}

void sendMessage()
{

  if (calc_delay)
  {
    SimpleList<uint32_t>::iterator node = nodes.begin();
    while (node != nodes.end())
    {
      mesh.startDelayMeas(*node);
      node++;
    }
    calc_delay = false;
  }

  taskSendMessage.setInterval(random(TASK_SECOND * 1, TASK_SECOND * 5)); // between 1 and 5 seconds
}

void receivedCallback(uint32_t from, String &msg)
{
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId)
{
  // Reset blink task
  onFlag = false;
  blinkNoNodes.setIterations((mesh.getNodeList().size() + 1) * 2);
  blinkNoNodes.enableDelayed(BLINK_PERIOD - (mesh.getNodeTime() % (BLINK_PERIOD * 1000)) / 1000);

  Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
  Serial.printf("--> startHere: New Connection, %s\n", mesh.subConnectionJson(true).c_str());
}

void changedConnectionCallback()
{
  Serial.printf("Changed connections\n");
  // Reset blink task
  onFlag = false;
  blinkNoNodes.setIterations((mesh.getNodeList().size() + 1) * 2);
  blinkNoNodes.enableDelayed(BLINK_PERIOD - (mesh.getNodeTime() % (BLINK_PERIOD * 1000)) / 1000);

  nodes = mesh.getNodeList();

  Serial.printf("Num nodes: %d\n", nodes.size());
  Serial.printf("Connection list:");

  SimpleList<uint32_t>::iterator node = nodes.begin();
  while (node != nodes.end())
  {
    Serial.printf(" %u", *node);
    node++;
  }
  Serial.println();
  calc_delay = true;
}

void nodeTimeAdjustedCallback(int32_t offset)
{
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}

void delayReceivedCallback(uint32_t from, int32_t delay)
{
  Serial.printf("Delay to node %u is %d us\n", from, delay);
}

// Recursive function to print the node tree
void printNodeTree(const painlessmesh::protocol::NodeTree &node, const String &prefix)
{
  Serial.println(prefix + String(node.nodeId));
  for (const auto &child : node.subs)
  {
    printNodeTree(child, prefix + "  "); // Indent for children
  }
}

// Function to log mesh details
void logDetailMesh()
{
  uint32_t nodeId = mesh.getNodeId();
  painlessmesh::protocol::NodeTree structure = mesh.asNodeTree();

  Serial.println("Root Node ID: " + String(nodeId));
  Serial.println("Connected Nodes: " + String(mesh.getNodeList().size()));
  Serial.println("Node Topology:");
  printNodeTree(structure, "");
  Serial.println(mesh.subConnectionJson());
  Serial.println();
}

void checkSerialInput()
{
  if (Serial.available())
  {
    String input = Serial.readStringUntil('\n');
    input.trim(); // Remove any trailing newline characters
    if (input == "topology")
    {
      logDetailMesh();
    }
    else if (input == "getTopology")
    {
      Serial.printf("###### Topology Start ###### \n\n");
      JsonDocument doc = getTopology(mesh);
      serializeJsonPretty(doc, Serial);
      Serial.printf("\n\n###### Topology End ###### \n\n");
    }
  }
}