// simulationManager_test.h

#include "common/taskmanager/taskmanager.h"




// A "Constant" Node
class TaskA : public Task {
private:
	int target;
	
public:
	
	int result;

	TaskA(int i)
	{
		target = i;
        std::string name = "Task A" + std::to_string(i);
		setName(&name);
	}

	bool isEqual(Task * t)
	{
		return false;
	}

	bool solve()
	{
		result = target;
		return true;
	}
	
};


// A Task class without any dependencies
class TaskB : public Task {
private:
	
public:
	int result;

	TaskB(int a1, int a2)
	{
      std::string name = "Task B";
		setName(&name);
		
		addDependency(new TaskA(a1));
		addDependency(new TaskA(a2));
	}

	bool isEqual(Task * t)
	{
		return true;
	}

	
	bool solve()
	{	
		Task * a1 = getDependencyRef(0);
		Task * a2 = getDependencyRef(1);
		result = static_cast<TaskA *>(a1)->result + static_cast<TaskA * >(a2)->result;
		return true;
	}
};

class TaskC : public Task {
private:
	int mult;

public:
	int result;

	TaskC(int a1, int a2, int newMult)
	{
      std::string name = "Task C";
		setName(&name);
		mult = newMult;
		addDependency(new TaskB(a1,a2));
	}

	bool isEqual(Task * t)
	{
		return true;
	}

	bool solve()
	{
		Task * B = getDependencyRef(0);
		result = mult* (static_cast<TaskB *>(B)->result);
		return true;
	}
};


TEST(TaskManagerTest, addTask)
{
	TaskManager m = TaskManager();

	int a1 = 2, a2 = 2, mult = 3;

	ASSERT_EQ(m.countTasks(), 0);
	
	m.addTask(new TaskC(a1,a2,mult));
	ASSERT_EQ(m.countTasks(), 4); // C, B, A1, A2
    

}

TEST(TaskManagerTest, solve)
{
	TaskManager m = TaskManager();

	int a1 = 4, a2 = 2, mult = 3;	
	Task * task = new TaskC(a1, a2, mult);
	m.addTask(task);

	m.solve();

	int r = static_cast<TaskC *>(task)->result;

	ASSERT_EQ(r, (a1 + a2)*mult);
}

TEST(TaskManagerTest, clean)
{
  TaskManager m = TaskManager();

  int a1 = 4, a2 = 2, mult = 3;
  Task * task = new TaskC(a1, a2, mult);
  
  m.addTask(task);
  
  ASSERT_EQ(m.countTasks(), 4);

  // Clean
  m.clean();

  ASSERT_EQ(0, m.countTasks());
}