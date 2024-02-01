#include <iostream>
#include <vector>
#include <memory>

using namespace std;

class Command{
public:
    char letter{};
    int time{}, team{}, id{}, arg{};
    Command(){
        cin >> time >> letter >> team >> id >> arg;
    }
    void Next(){
        cin >> time >> letter >> team >> id >> arg;
    }
};
class Robot{
public:
    int team{}, id{}, isAlive=1, level=1, maxHP{}, maxHT{}, HP{}, HT{};
    void Broken(){
        isAlive = 0;
        cout << "D " << this->team << " " << this->id << endl;
    }
    void Revive(){
        if(isAlive == 0) {
            isAlive = 1;
            HP = maxHP;
            HT = 0;
        }
    }
    void Damage(int damage){
        if(isAlive == 1) {
            this->HP -= damage;
            if (this->HP <= 0)
                Broken();
        }
    }
    virtual void HeatUp(int heat) = 0;
    virtual void LevelUp(int level) = 0;
};
class Infantry : public Robot{
public:
    Infantry(int team, int id){
        this->team = team;
        this->id = id;
        maxHP = HP = 100;
        maxHT = 100;
        HT = 0;
    }
    void HeatUp(int heat) override{
        HT += heat;
    }
    void LevelUp(int level) override{
        if(isAlive == 1 && level > this->level){
            if(level == 2){
                this->level = 2;
                maxHP = HP = 150;
                maxHT = 200;
            }else if(level == 3){
                this->level = 3;
                maxHP = HP = 250;
                maxHT = 300;
            }
        }

    }
};
class Engineer : public Robot{
public:
    Engineer(int team, int id){
        this->team = team;
        this->id = id;
        maxHP = HP = 300;
        maxHT = HT = 0;
    }
    void HeatUp(int heat) override{}
    void LevelUp(int level) override{}
};
class Manage{
    vector<shared_ptr<Robot>> Robots;
    void Add(int team, int id, int type){
        Robot *Rptr;
        if(type == 0)
            Rptr = new Infantry(team, id);
        else
            Rptr = new Engineer(team, id);
        shared_ptr<Robot> R(Rptr);
        Robots.push_back(R);
    }
    shared_ptr<Robot> Find(int team, int id){
        for(shared_ptr<Robot> R : Robots)
            if(R->team == team && R->id == id)
                return R;
        return nullptr;
    }
public:
    void DoCommand(Command &C){
        shared_ptr<Robot> R = Find(C.team, C.id);
        switch (C.letter) {
            case 'A':
                if(R == nullptr)
                    Add(C.team, C.id, C.arg);
                else
                    R->Revive();
                break;
            case 'F':
                if(R != nullptr)
                    R->Damage(C.arg);
                break;
            case 'H':
                if(R != nullptr)
                    R->HeatUp(C.arg);
                break;
            case 'U':
                if(R != nullptr)
                    R->LevelUp(C.arg);
        }
    }
    void Update(){
        for(shared_ptr<Robot> &R : Robots){
            if(R->isAlive == 1 && R->HT > 0){
                if(--R->HT > R->maxHT){
                    R->HP -= R->HT - R->maxHT;
                    if(R->HP <= 0)
                        R->Broken();
                }
            }
        }
    }
};




int main() {
    Manage Mng = Manage();
    int N, t=0;
    cin >> N;
    Command thisCommand = Command();
    do{
        while(t == thisCommand.time){
            Mng.DoCommand(thisCommand);
            if(--N > 0)
                thisCommand.Next();
            else
                thisCommand.time = 65536;
        }
        Mng.Update();
    }while(++t < 65536);
    return 0;
}
