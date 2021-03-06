/*---------------------------------------------------------------------------------------------------------------------
Function: 1.User Login  2.Asking questions 3.Answer questions 4.Upvote answer 5.Follow User
----------------------------------------------------------------------------------------------------------------------*/
#include<iostream>
#include<fstream>
#include<iomanip>
#include<string>
#include<vector>
#include<regex>
#include<time.h>

using namespace std;

fstream user;
fstream questions;
fstream answers;
std::vector<char> curuser;
//Basicinfo Class
class Basicinfo{
        
public:
    virtual void CreatQuestionAndAnswer(int randomnum)  = 0;
    // Basicinfo();
    // ~Basicinfo();
    void run();
    int  Login();
    void List();
    void Upvote(int ID);
    void Follow(int ID);
    void Logout();
    int  Signup();
    void SelectQuestion();
    void search();
    void ListFollower();
};


struct userdata{
  char username[20];
  char password[20];
  char focuslist[20];

};

//Question Class
class Question:public Basicinfo{
public:
  int ID;
  void CreatQuestionAndAnswer(int randomnum) ;
  char questioner[20];
  char questionslist[200];

};

//Answer Class
class Answer:public Basicinfo{
public:
   int ID;
   void CreatQuestionAndAnswer(int randomnum) ;
   char answerslist[200];
   char answerer[20];
  
   int upvotenum(){
     return this->upvoteNum;
   }

   void upvote()
   {
     this->upvoteNum ++;
   }

private: 
   int upvoteNum = 0; 
};


//Function:Signup
int Basicinfo::Signup(){
	userdata us;
    std::string username{"\\w{8}"};
    std::regex reuser(username);
    std::string password{"\\d{8}"};
    std::regex repassword(password);
    char inuser[20],inpassword[20];

    user.open("user.dat",ios::in | ios::out | ios::binary | ios::app);
    if (!user.is_open())
      cout << "Open file error!" << endl;

    cout << "请输入用户名:(以8个大小写字母或数字组成)" << endl;
    cin >> inuser;
    if (!regex_match(inuser,reuser)){
      cout << "请输入正确格式的用户名！";
      cin >> inuser;
    }else{
		strcpy(us.username, inuser);
    }

    cout << "请输入密码:(以8个数字组成)" << endl;
    cin >> inpassword;
    if (!regex_match(inpassword,repassword)){
      cout << "请输入正确格式的密码" << endl;
	  cin >> inpassword;
    }else {
		strcpy(us.password, inpassword);
    }
	
    user.write((char*)&us,sizeof(userdata));
    user.close();
    return 1;
}

//Function:Login
int Basicinfo::Login(){
  char username[20],password[20];
  userdata us;

  int temp = 0; //用于记录用户名是否存在
  cout << "请输入用户名：" << endl;
  cin >> username ;

  user.open("user.dat",ios::in | ios::out | ios::binary | ios::app);
    if (!user.is_open())
      cout << "Open file error!" << endl;
  user.seekg(0);
  user.read((char*)&us,sizeof(userdata));

  while (!user.eof()){
    if (strcmp(us.username,username) == 0){
	  temp = 1;
      cout << "请输入密码：" << endl;
      cin >> password;
      while (strcmp(us.password,password) != 0){
        cout << "请输入正确的密码" << endl;
		cin >> password;
      }
	  break;
    }else{
      user.read((char*)&us,sizeof(userdata));
    }
  }
  for (int i =0;us.username[i] != '\0';i++)
    curuser.push_back(us.username[i]);
  //用户名是否存在功能
  if (temp == 0)
	  cout << "用户名不存在!" << endl;

  user.close();
  return temp;
}

//Function:Logout
void Basicinfo::Logout(){
  Answer an;
  Question qe;
  qe.run();
}

//Function:list all of the questions and answers
void Basicinfo::List(){
  
  Question qe ;
  Answer an;

  questions.open("question.dat",ios::in | ios::out | ios::binary | ios::app);
    if (!questions.is_open())
      cout << "Open file error!" << endl;
  
  answers.open("answers.dat",ios::in | ios::out | ios::binary | ios::app);
    if (!answers.is_open())
      cout << "Open file error!" << endl;

  questions.seekg(0);
  answers.seekg(0);
  questions.read((char*)&qe,sizeof(Question));
  answers.read((char*)&an,sizeof(Answer));

  for (int i = 1; !questions.eof()&& !answers.eof(); ++i){
	  cout << "问题";
	  cout << i << "." << '\t';
    cout << qe.questionslist <<endl;
    while (!answers.eof()){
      if (an.ID == qe.ID){
		int j = 1;
		cout << "回答" ;
		cout << j << "." << '\t';
        cout << an.answerslist << '\t';
		cout << "回答者:" << '\t';
        cout << an.answerer << endl;
		j++;
      }
      answers.read((char*)&an,sizeof(Answer));
    }
    questions.read((char*)&qe,sizeof(Question));
	answers.clear();
  answers.seekg(0);
	answers.read((char*)&an, sizeof(Answer)); 
  }
  qe.SelectQuestion();
  questions.close();
  answers.close();
}

//Function:search questions and answers
void Basicinfo::search(){
  Question qe;
  Answer an;
  string tempstring;

  questions.open("question.dat",ios::in | ios::out | ios::binary | ios::app);
    if (!questions.is_open())
      cout << "Open file error!" << endl;
  answers.open("answers.dat",ios::in | ios::out | ios::binary | ios::app);
    if (!answers.is_open())
      cout << "Open file error!" << endl;
  
  cout << "请输入要查找的问题:" << endl;
  getline(cin,tempstring);

  questions.seekg(0);
  questions.read((char*)&qe,sizeof(Question));
  answers.seekg(0);
  answers.read((char*)&an,sizeof(Answer));
  
  string str = qe.questionslist;
  while(!questions.eof()){
      if (str.find(tempstring) != str.npos){
        cout << "问题:" << '\t';
        cout << qe.questionslist << endl;
        while (!answers.eof()){
           if (an.ID == qe.ID){
              cout << "回答:" << '\t';
              cout << an.answerslist <<'\t';
              cout << an.answerer <<'\t';
              cout << "点赞数目:" <<'\t';
              cout << an.upvotenum() << endl;
              answers.read((char*)&an,sizeof(Answer));
           }
           else{
              answers.read((char*)&an,sizeof(Answer));
           }   
        }
		break;
      }
      else
      {
        questions.read((char*)&qe,sizeof(Question));
        str = qe.questionslist;
      }
  }
  questions.close();
  answers.close();
}

//Function:List followers
void Basicinfo::ListFollower(){
  Question qe;
  Answer an;
  userdata us;
  char temparray[20];
  int i = 0;
  //将当前用户存进数组
  memset(temparray, 0, sizeof(temparray));
  for (vector<char>::iterator iter = curuser.begin();iter != curuser.end() && temparray; ++iter,++i)
    temparray[i] = *iter;

  questions.open("question.dat",ios::in | ios::out | ios::binary | ios::app);
    if (!questions.is_open())
      cout << "Open file error!" << endl;
  
  answers.open("answers.dat",ios::in | ios::out | ios::binary | ios::app);
    if (!answers.is_open())
      cout << "Open file error!" << endl;

  user.open("user.dat",ios::in | ios::out | ios::binary | ios::app);
    if (!user.is_open())
      cout << "Open file error!" << endl;

  questions.seekg(0);
  questions.read((char*)&qe,sizeof(Question));
  answers.seekg(0);
  answers.read((char*)&an,sizeof(Answer));
  user.seekg(0);
  user.read((char*)&us,sizeof(userdata));

  while(!user.eof()){
    if (strcmp(us.username,temparray) == 0){
      for ( ;!questions.eof() && !answers.eof(); questions.read((char*)&qe,sizeof(Question)),answers.read((char*)&an,sizeof(Answer))){
        if (strcmp(qe.questioner,us.focuslist) == 0){
          cout << "关注者所提问题:" << '\t';
          cout << qe.questionslist << endl;
        }
        if (strcmp(an.answerer,us.focuslist) == 0){
          cout << "关注者的回答:" << '\t';
          cout << an.answerslist << endl;
        }
      }
    break;
    }
    else
      user.read((char*)&us,sizeof(userdata));
  }

}


//virtual funtion :: Answer
void Answer::CreatQuestionAndAnswer(int randomnum){
  Answer an;
  char answer[200];
  int i = 0;
  cout << "请输入答案：" <<endl;
  cin >> answer;
  strcpy(an.answerslist,answer);
  //随机生成一个ID来标签问题和答案
  an.ID = randomnum;
  //记录当前回答者
  memset(an.answerer, 0, sizeof(an.answerer));
  for (vector<char>::iterator iter = curuser.begin();iter != curuser.end() && an.answerer; ++iter,++i)
    an.answerer[i] = *iter;
  

  answers.open("answers.dat",ios::in | ios::out | ios::binary | ios::app);
    if (!answers.is_open())
      cout << "Open file error!" << endl;
    
  answers.write((char*)&an,sizeof(Answer));
  answers.close();
}

//virtual function :: Question
void Question::CreatQuestionAndAnswer(int randomnum){
  Question qe;
  char question[200];
  int i = 0;
  cout << "请输入问题：" << endl;
  cin >> question;
  strcpy(qe.questionslist,question);
  //随机生成一个ID来标签问题和答案
  qe.ID = randomnum;
  //记录当前提问者
  memset(qe.questioner, 0, sizeof(qe.questioner));
  for (vector<char>::iterator iter = curuser.begin();iter != curuser.end() && qe.questioner; ++iter,++i)
    qe.questioner[i] = *iter;

  questions.open("question.dat",ios::in | ios::out | ios::binary | ios::app);
    if (!questions.is_open())
      cout << "Open file error!" << endl;
  questions.write((char*)&qe,sizeof(Question));
  questions.close();
}

//Funcrion:upvote
void Basicinfo::Upvote(int ID) {
	Answer an;
	Question qe;
	int i;

	cout << "请选择你的操作: 1.点赞该回答 2.关注回答者 3.继续" << endl;
	cin >> i;

	if (i == 1) {
    ofstream tempfile("temp.dat",ios::out | ios::app | ios::binary);
		answers.clear();
		answers.seekg(0);
		answers.read((char*)&an, sizeof(Answer));
		while (!answers.eof()) {
			if (an.ID == ID) {
				an.upvote();
				tempfile.write((char*)&an, sizeof(Answer));
			}
			else {
				tempfile.write((char*)&an, sizeof(Answer));
			}
			answers.read((char*)&an, sizeof(Answer));
		}
		answers.close();
		tempfile.close();

		remove("answers.dat");
		rename("temp.dat", "answers.dat");
		answers.open("answers.dat", ios::in | ios::out | ios::app);
	}
  if (i == 2){
    an.Follow(ID);
  }
}

//Function:follow
void Basicinfo::Follow(int ID){
  Answer an;
  userdata us;

  char temparray[20];
  int i = 0;
  //将当前用户存进数组
  memset(temparray, 0, sizeof(temparray));
  for (vector<char>::iterator iter = curuser.begin();iter != curuser.end() && temparray; ++iter,++i)
    temparray[i] = *iter;

  ofstream tempfile("temp.dat",ios::out | ios::app | ios::binary);

  user.open("user.dat",ios::in | ios::out | ios::binary | ios::app);
    if (!user.is_open())
      cout << "Open file error!" << endl;

  answers.clear();
  answers.seekg(0);
  answers.read((char*)&an,sizeof(Answer));
  user.seekg(0);
  user.read((char*)&us,sizeof(userdata));

  while (!user.eof()){
    if (strcmp(us.username,temparray) == 0){
      while (!answers.eof()){
         if (an.ID == ID){
            strcpy(us.focuslist,an.answerer);
            tempfile.write((char*)&us,sizeof(userdata));
         }
         answers.read((char*)&an,sizeof(Answer));
      }
	  user.read((char*)&us, sizeof(userdata));
    }
	else {
	  tempfile.write((char*)&us, sizeof(userdata));
	  user.read((char*)&us, sizeof(userdata));
	}
  }
  
  user.close();
  tempfile.close();

  remove("user.dat");
  rename("temp.dat","user.dat");
}

//Function:select questions
void Basicinfo::SelectQuestion(){
  Question qe;
  Answer an;
  int num;

  cout << "请选择你想查看的问题序号：" << endl;
  cin >> num;

  questions.clear();
  questions.seekg(0);
  questions.seekg((int)((num-1) * sizeof(Question)));
  answers.seekg(0);
  questions.read((char*)&qe,sizeof(Question));
  answers.read((char*)&an,sizeof(Answer));

  cout << "问题:" << '\t';
  cout << qe.questionslist << endl;

  while (!answers.eof()){
    if(an.ID == qe.ID){
		cout << "回答:" << '\t';
      cout << an.answerslist << '\t';
	  cout << "回答者:" << '\t';
      cout << an.answerer << '\t';
      cout << "点赞数目:" << an.upvotenum() <<endl;
    }
    answers.read((char*)&an,sizeof(Answer));
  }
  an.Upvote(qe.ID);
}

//function:run the user interface
void Basicinfo::run() {
	system("cls");
	int i;
	int j = 1;
	Answer an;
	Question qe;

	srand((unsigned)time(NULL));
	int randomnum = rand() % 10000;

	do
	{
		cout << setw(20) << "------------------------------欢迎来到瞎扯系统---------------------" << endl;
		cout << setw(20) << "------------------------------1.注册-------------------------------" << endl;
		cout << setw(20) << "------------------------------2.登陆-------------------------------" << endl;

		cin >> i;

		while (cin.get() != '\n');

		switch (i)
		{
		case 1:
			qe.Signup();
			break;
		case 2:
			j = qe.Login();
			break;

		}
	} while (j == 0);
		system("cls");
		do
		{
			cout << setw(20) << "--------------------------------瞎扯---------------------------------" << endl;
			cout << setw(20) << "------------------------------1.查看问题与回答-----------------------" << endl;
			cout << setw(20) << "------------------------------2.创建问题-----------------------------" << endl;
			cout << setw(20) << "------------------------------3.创建回答-----------------------------" << endl;
			cout << setw(20) << "------------------------------4.搜索问题与回答-----------------------" << endl;
			cout << setw(20) << "------------------------------5.查看关注用户---------------------------------" << endl;
			cout << setw(20) << "------------------------------6.注销---------------------------------" << endl;
			cout << setw(20) << "------------------------------7.退出---------------------------------" << endl;

			cin >> i;

			while (cin.get() != '\n');

			switch (i)
			{
			case 1:
				qe.List();
				break;
			case 2:
				qe.CreatQuestionAndAnswer(randomnum);
				break;
			case 3:
				an.CreatQuestionAndAnswer(randomnum);
				break;
			case 4:
				qe.search();
				break;
			case 5:
				qe.ListFollower();
				break;
			case 6:
				qe.Logout();
				break;
			case 7:
				break;
			}
		} while (i != 7);
	}


int main (void){
	Question qe;
	Answer an;
	an.run();
}
