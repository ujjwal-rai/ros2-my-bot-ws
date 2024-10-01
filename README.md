# ros2-my-bot-ws
To add your ROS2 project (`ros2-my-bot-ws`) to a GitHub repository, I will walk you through the entire procedure, explaining how to set up the workspace, build it, and include key components like URDF files, launch files, and integration with Gazebo simulation. Here is a step-by-step guide.

---

## **Procedure to Add the ROS2 Project to GitHub Repository**

### **1. Set Up Your ROS2 Workspace**
Start by creating the ROS2 workspace and the necessary files for your bot.

1. **Navigate to Your Workspace Directory**:
   ```bash
   cd ~/ros2-my-bot-ws
   ```

2. **Create Folders for the URDF, Launch, and Gazebo Files**:
   Inside your workspace, ensure you have the necessary directories for URDF models, launch files, and other robot-related resources.
   ```bash
   mkdir -p src/my_robot/urdf
   mkdir -p src/my_robot/launch
   mkdir -p src/my_robot/worlds
   ```

### **2. Writing the URDF File**

The **URDF (Unified Robot Description Format)** defines the physical model of your robot. It describes the robot's joints, links, geometry, and physical properties such as inertia, mass, and visual elements.

Create a `my_robot.urdf` file inside the `urdf` folder:
```bash
nano src/my_robot/urdf/my_robot.urdf
```

Example URDF content:
```xml
<robot name="my_robot">
  <link name="base_link">
    <visual>
      <geometry>
        <box size="0.5 0.5 0.2"/>
      </geometry>
      <material name="blue"/>
    </visual>
  </link>

  <joint name="base_to_wheel" type="continuous">
    <parent link="base_link"/>
    <child link="wheel_link"/>
    <origin xyz="0 0 0" rpy="0 0 0"/>
    <axis xyz="0 0 1"/>
  </joint>

  <link name="wheel_link">
    <visual>
      <geometry>
        <cylinder length="0.05" radius="0.1"/>
      </geometry>
      <material name="black"/>
    </visual>
  </link>
</robot>
```
This is a basic URDF model describing a simple box as a robot base and a cylindrical wheel. You can modify this to suit your robot's design.

### **3. Writing a Launch File**

Launch files in ROS2 are used to launch multiple nodes and configurations for simulations. They are typically written in Python or XML.

Create a launch file for your bot:
```bash
nano src/my_robot/launch/my_robot_launch.py
```

Example launch file (Python):
```python
from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='robot_state_publisher',
            executable='robot_state_publisher',
            output='screen',
            parameters=[{'use_sim_time': True}],
            arguments=['src/my_robot/urdf/my_robot.urdf']
        )
    ])
```
This launch file starts the **`robot_state_publisher`** node, which publishes the state of the robot as described in the URDF file.

### **4. Integrating Gazebo**

Gazebo is a popular robot simulation tool that integrates with ROS2 to simulate the robot's physics and behavior in a virtual environment.

1. **Creating a Gazebo World File**:
   Create a basic world file for Gazebo inside the `worlds` directory.
   ```bash
   nano src/my_robot/worlds/my_robot_world.world
   ```

   Example world file:
   ```xml
   <sdf version="1.6">
     <world name="default">
       <include>
         <uri>model://sun</uri>
       </include>
       <include>
         <uri>model://ground_plane</uri>
       </include>
     </world>
   </sdf>
   ```
   This world file adds a ground plane and sunlight to your Gazebo simulation.

2. **Creating a Gazebo Launch File**:
   Create a launch file for Gazebo in the `launch` directory to simulate your robot in Gazebo.
   ```bash
   nano src/my_robot/launch/gazebo_launch.py
   ```

   Example launch file:
   ```python
   from launch import LaunchDescription
   from launch_ros.actions import Node
   from launch.actions import ExecuteProcess

   def generate_launch_description():
       return LaunchDescription([
           ExecuteProcess(
               cmd=['gazebo', '--verbose', 'src/my_robot/worlds/my_robot_world.world', '-s', 'libgazebo_ros_factory.so'],
               output='screen'
           ),
           Node(
               package='gazebo_ros',
               executable='spawn_entity.py',
               arguments=['-file', 'src/my_robot/urdf/my_robot.urdf', '-entity', 'my_robot'],
               output='screen'
           )
       ])
   ```

   This launch file starts the Gazebo simulation and spawns your robot inside the defined world.

### **5. Building the Workspace**

After setting up all the files, you need to build the workspace using `colcon`.

```bash
colcon build --symlink-install
```

- `--symlink-install`: This option creates symlinks instead of copying the files, which helps during development.

### **6. Testing the Launch Files**

1. **Test Robot State Publisher Launch**:
   ```bash
   ros2 launch my_robot my_robot_launch.py
   ```

2. **Test Gazebo Launch**:
   ```bash
   ros2 launch my_robot gazebo_launch.py
   ```

This will launch Gazebo and spawn your robot as per the URDF file.

---


### **Summary of Key Components**

- **URDF File**: Defines the robot's structure, geometry, and physical properties. It is used for visualizing and simulating the robot in RViz and Gazebo.
- **Launch Files**: Used to start nodes and processes in ROS2. They are responsible for launching your robot model in simulation environments or actual robot systems.
- **Gazebo Integration**: Gazebo is a powerful robot simulation tool integrated with ROS2, allowing you to test your robot in a virtual environment. The robot can be spawned in a Gazebo world using launch files.
