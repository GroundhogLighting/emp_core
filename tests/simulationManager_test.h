// simulationManager_test.h

#include "../src/calculator/src/simulationManager.h"




// A Task class without any dependencies
class TaskA : public Task {
private:

public:
	TaskA()
	{
		setName("Task A");
		setID(1);
	}
};


// A Task class without any dependencies
class TaskB : public Task {
private:
	
public:
	TaskB()
	{
		setName("Task B");
		setID(2);

		addDependency(new TaskA());
	}
};

class TaskC : public Task {
private:

public:
	TaskC()
	{
		setName("Task C");
		setID(3);

		addDependency(new TaskB());
	}
};


class TaskD : public Task {
private:

public:
	TaskD()
	{
		setName("Task D");
		setID(4);

		addDependency(new TaskB());
	}
};


TEST(SimulationManagerTest, addTask)
{
	SimulationManager sm = SimulationManager();

	ASSERT_EQ(sm.countTasks(), 0);

	sm.addTask(new TaskB(),NULL);
	ASSERT_EQ(sm.countTasks(), 2);
	sm.print();


	sm.addTask(new TaskC(),NULL);
	sm.print();
	ASSERT_EQ(sm.countTasks(), 3);

	sm.addTask(new TaskD(), NULL);
	sm.print();
	ASSERT_EQ(sm.countTasks(), 4);


}