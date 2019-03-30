#include <QCoreApplication>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Actuator
{
public:
    Actuator(){}
    virtual ~Actuator(){}
    virtual void dowork() = 0;
};

class Qx30uActuator: public Actuator
{
public:
    Qx30uActuator(){}
    virtual ~Qx30uActuator(){}
    virtual void dowork()
    {
        cout<<"Qx30U Action"<<endl;
    }

};

class UvcActuator: public Actuator
{
public:
    UvcActuator(){}
    virtual ~UvcActuator(){}
    virtual void dowork()
    {
        cout<<"UVC Action"<<endl;
    }

};



class Command
{
protected:
    Actuator* m_master;
public:
    Command(Actuator* p)
    {
        m_master = p;
    }
    virtual void executeCmd() = 0;
};

class CommandQx30u : public Command
{
public:
    CommandQx30u(Actuator* p) : Command(p) {}
    void executeCmd()
    {
        m_master->dowork();
    }
};

class CommandUvc : public Command
{
public:
    CommandUvc(Actuator* p) : Command(p) {}
    void executeCmd()
    {
        m_master->dowork();
    }
};

class Coordinator
{
private:
    std::vector<Command *> m_vec;
public:
    ~Coordinator()
    {
        for (auto it = m_vec.begin(); it != m_vec.end(); it++)
        {
            delete(*it);
        }
        m_vec.clear();
    }
    void add(Command * p)
    {
        m_vec.push_back(p);
    }
    void remove(Command * p)
    {
        auto it = find(m_vec.begin(), m_vec.end(), p);
        if (it != m_vec.end())
            m_vec.erase(it);
    }
    void submitCmd()
    {
        for (auto it = m_vec.cbegin(); it != m_vec.cend(); it++)
        {
            (*it)->executeCmd();
        }
    }
};


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    cout<<"DP23, Command Pattern"<<endl;

    Coordinator* pCoordinator = new Coordinator(); //passer
    Actuator* pQxMaster = new Qx30uActuator(); //executor
    Actuator* pUvcMaster = new UvcActuator();  //executor
    pCoordinator->add(new CommandQx30u(pQxMaster)); //Add new command
    pCoordinator->add(new CommandUvc(pUvcMaster));  //Add new command
    pCoordinator->submitCmd(); //Submit command
    delete pQxMaster;
    delete pUvcMaster;
    delete pCoordinator;

    return a.exec();
}
