#include <iostream>
#include <set>  //set
#include <climits> //INT_MAX
#include <sstream> //stringstream
#include <map> //map
#include <fstream> //ifstream
#include <cstring> //strcmp



#define Esc 27

using namespace std;

// define actions as enums
enum Action {LEFT,RIGHT,ACCEPT, REJECT, NOT_DEFINED};
// outputs r in the form of structs of 3 (state,input,action)
struct OutPut
{
    int state = -1;
    char alphabet = '\n';
    Action action = NOT_DEFINED;
};
// number of states
int numOfStates;
// list of inputs
set<char> inputs;
// 2D list of outputs
map<pair<int,char>,OutPut> outputs;
// create function to execute action
void execute_action();

void init_states();
void take_input();
void take_rules_from_user();
void clear_cin();
Action translateAction(char ch);
bool correct_output(OutPut op);
bool read_rules_file();
bool evaluate(char* TMinputPtr, int startingState, bool withSteps);
void printRule(int inputState, char inputCharacter, int outputState, char outputCharacter, char action);

void trying()
{

//    for(int i = 0; i < 50; i++)
//        cout << "\e[0;"<<i<<"m"<<"testing! "<< i <<  endl;

//    init_states();
//    take_input();
//    take_rules_from_user();

//    cout << read_rules_file() << endl;
//    char x [] = ">aaababb";
//    cout << evaluate(x, 0, true) << endl;

//    cout << "NOTE: Input must starts with > (bigger than) symbol! " << endl;
//    char line [128];
//    int startingState = 0;
//    char withSteps;
//    while(true){
//        cout << "Enter the input to the Turing Machine or (Q/q) to exit: " << endl;
//        cin >> line;
//        if(strcmp(line,"q") ==0 && strcmp(line,"Q") ==0) break;
//        cout << "Starting state: ";
//        cin >> startingState;
//        cout << "Show steps? (Y,y) ";
//        cin >> withSteps;
////        if(evaluate(line,startingState, withSteps=='Y'||withSteps=='y'))
////            cout << "Accepted!" << endl;
////        else
////            cout << "Rejected!" << endl;
//        cout << line << startingState << withSteps << endl;
//        clear_cin();
//
//    }
//
//    cout << "Exiting Turing Machine!" << endl;

}

int main()
{
    trying();
//    return 0;

    cout << "Did u write the rules in the \'Rules.txt\' file? \e[5m(Y/N)\e[0m ";
    char choice;
    switch(choice = cin.get())
    {
    case 'Y':
    case 'y':
        read_rules_file();
        break;

    default:
        init_states();
        take_input();
        take_rules_from_user();
    }

    cout << "NOTE: Input must starts with \e[1m > \e[4m(bigger than)\e[0m symbol! " << endl;
    char line [128];
    int startingState = 0;
    char withSteps;
    while(true){
        clear_cin();
        cout << "Enter the input to the Turing Machine or \e[1;5m(Q/q)\e[0m to exit: " << endl;
        cin >> line;
        if(strcmp(line,"q") ==0 && strcmp(line,"Q") ==0) break;
        cout << "Starting state: ";
        cin >> startingState;
        if(!cin.good()) continue;
        cout << "Show steps? (Y,y) ";
        cin >> withSteps;
        if(evaluate(line,startingState, withSteps=='Y'||withSteps=='y'))
            cout << "Accepted!" << endl;
        else
            cout << "Rejected!" << endl;
    }

    cout << "Exiting Turing Machine!" << endl;
    return 0;
}

void init_states()
{
    cout << "please enter the number of states: ";
    cin >> numOfStates;
}

void take_input()
{
    cout << "please enter the alphabet separated by (Enter) and \e[4m(Esc)(Enter) when u r done\e[0m: "
         << endl << "note the if u enter more than 1 charater, only the 1st will be taken!" << endl;
    char ch;
    while((ch = cin.get()) != Esc)
    {
        if(ch == '\n') continue;
        inputs.insert(ch);
        while((ch = cin.get()) != '\n');
    }
    clear_cin();
}

void clear_cin()
{
    cin.clear();
    cin.ignore(INT_MAX,'\n'); // ignore all till \n (included)
}

Action translateAction(char ch)
{
    switch(ch)
    {
    case 'L':
    case 'l':
        return LEFT;
    case 'R':
    case 'r':
        return RIGHT;
    case 'Y':
    case 'y':
        return ACCEPT;
    case 'N':
    case 'n':
        return REJECT;
    }
    return NOT_DEFINED;
}

void take_rules_from_user()
{
    int ruleCount = numOfStates * inputs.size();
    cout << "\e[36;2mExpected " << ruleCount <<" rule!"
         "If the rule is \'(s,r) => (s,w,a)\'\n"
         "Write just the \'s w a\':\e[0m\n";
    stringstream ss;
    string line;
    for(int i = 0; i < numOfStates; i++ )
        for(set<char>::iterator itr = inputs.begin(); itr != inputs.end(); itr++)
        {
            bool isGood = false;
            do
            {
                cout << "( " << i << ", " << *itr << ") => ";
                OutPut output;
                char action ='\n';
                getline(cin, line);
                ss.str(line);
                ss >> output.state >> output.alphabet >> action;
                output.action = translateAction(action);
                if(correct_output(output))
                {
                    outputs[make_pair(i, *itr)] = output;
                    isGood = true;
                }
                else
                    cout << "This rule is wrong re-run the program!"<<endl;
                ss.clear();
            }
            while(!isGood);
            //if they r separated well (Not on the right form) correct_output() will return false as well
        }
}

bool correct_output(OutPut op)
{
    if(op.action == NOT_DEFINED || op.state > numOfStates || op.state < 0) return false;
    return inputs.find(op.alphabet) != inputs.end();
}

bool read_rules_file()
{
    cout << "\e[1;31;2mWarning Rules file errors are NOT handled!\nDouble check your file!!!\e[0m" <<endl;
    ifstream rules_file("Rules.txt");
    if(!rules_file.is_open())
    {
        cout << "\e[41mError: file is not open!\e[0m" << endl;
        return false;
    }
    string line;
    stringstream ss;
    while(getline(rules_file, line))
    {
        if(line == "" || line[0] == '.') continue;
        ss.str(line);
        OutPut output;
        int inputState;
        char inputCharacter, action;
        if (!(ss >> inputState >> inputCharacter >> output.state >> output.alphabet >> action))
        {
            cout << "\e[41mError while reading next input!\e[0m" << endl;
            return false;
        }
        printRule(inputState, inputCharacter, output.state, output.alphabet, action);
        output.action = translateAction(action);
        outputs[make_pair(inputState, inputCharacter)] = output;
        ss.clear();
    }
    rules_file.close();
    return true;
}

bool evaluate(char* TMinputPtr, int startingState, bool withSteps)
{
    int itr = 0;
    char* theInputStr = TMinputPtr;
    if(*TMinputPtr != '>')
    {
        cout << "\e[41mError: the head symbol(\e[1m>\e[0m) can be found!\e[0m" << endl;
        return false;
    }
    OutPut next;
    next = outputs[make_pair(startingState, *(++TMinputPtr))];
    if(withSteps)
    {
        printRule(startingState, *TMinputPtr, next.state, next.alphabet, translateAction(next.action));
        printf("%d) %s\n",itr++ ,theInputStr);
    }
    while(true)
    {
        *TMinputPtr = next.alphabet;
        switch(next.action)
        {
        case LEFT:
            TMinputPtr--;
            if(*TMinputPtr == '>')
            {
                cout << "\e[44mError: machine reached the head symbol(>)\e[0m" << endl;
                return false;
            }
            break;
        case RIGHT:
            TMinputPtr++;
            break;
        case ACCEPT:
            return true;
        case REJECT:
            return false;
        default:
        case NOT_DEFINED:
            cout << "\e[41mError: NOT_DEFINED ACTION!\e[0m"<< endl;
            return false;
        }

        next = outputs[make_pair(next.state, *TMinputPtr)];
        if(withSteps)
        {
            printRule(startingState, *TMinputPtr, next.state, next.alphabet, translateAction(next.action));
            printf("%d) %s\n",itr++ ,theInputStr);
        }
    }
    return false;
}

void printRule(int inputState, char inputCharacter, int outputState, char outputCharacter, char action)
{
    cout <<"("<< inputState <<", "<< inputCharacter <<") => ("<< outputState <<", "<< outputCharacter <<", "<< action <<")"<< endl;
}

char translateAction(Action action)
{
    switch(action)
    {
    case LEFT:
        return 'L';
    case RIGHT:
        return 'R';
    case ACCEPT:
        return 'Y';
    case REJECT:
        return 'N';
    default:
        return 'X';
    }
    return 'X';
}

