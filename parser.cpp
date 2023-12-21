#include <bits/stdc++.h>
using namespace std;

template<class T>
void printall(T arr){
  for(auto x:arr){
    cout<<x<<" ";
  }
  cout<<endl;
}

class not_ll1_exception:public exception{
public:
  string msg;
  not_ll1_exception(const string& s)noexcept :msg(s) {
    
  }
  const char* what(){
    return msg.c_str();
  }
};


struct Parser{
  static const string eps;
  static const string eol;
  string start_symbol;
  map<string,vector<vector<string>>> rules;
  map<string,set<string>> firsts,follows;
  set<string> terminals,nullables;
  map<string,map<string,int>> parse_table;

private:
  // map<string,int> __nullmap;

public:
  void reset_parameters(){
    rules.clear();
    firsts.clear();
    follows.clear();
    terminals.clear();
    nullables.clear();
    start_symbol.clear();
    parse_table.clear();

  }

  void display_first(){
    cout<<"::::::FIRST CALCULATION::::::"<<endl;
    for(const auto& p:firsts){
      cout<<"Symbol("<<p.first<<"): "<<endl;
      for(const auto& t:p.second){
        cout<<t<<" ";
      }
      cout<<endl;
    }
  }

  void display_follow(){
    cout<<"::::::FOLLOW CALCULATION::::::"<<endl;
    for(const auto& p:follows){
      cout<<"Symbol("<<p.first<<"): "<<endl;
      for(const auto& t:p.second){
        cout<<t<<" ";
      }
      cout<<endl;
    }
  }

  void display_parse_table(){
    cout<<"::::::PARSE TABLE::::::"<<endl;
    set<string> all_terminals;

    for(const auto& p:parse_table){
      cout<<"Rule("<<p.first<<"): "<<endl;
      for(const auto& t:p.second){
        all_terminals.insert(t.first);
        cout<<"("<<t.first<<") "<<t.second<<endl;
      }
    }

    cout<<"::::::NEW PARSE TABLE::::::"<<endl;
    cout<<"terminals-> "<<endl;
    for(const auto& t:all_terminals){
      cout<<t<<" ";
    }
    cout<<endl;
    for(const auto& p:parse_table){
      cout<<p.first<<"-> ";
      for(const auto& t:all_terminals){
        if(!p.second.count(t)){
          cout<<-1<<" ";
        }else{
          // auto t1(p.second.);
          cout<<p.first<<"_"<<(int)(p.second.find(t)->second)<<" ";
        }
      }
      cout<<endl;
    }
  }

  void display_nullables(){
    cout<<"::::::NULLABLES::::::"<<endl;
    for(const auto& t:nullables){
      cout<<t<<" ";
    }
    cout<<endl;
  }

  void setup_parser(const string& rule_file,const string& terminal_file,bool display=false){
    //call the full pipeline here
    reset_parameters();
    read_rules_from_file(rule_file,terminal_file);
    get_nullables();
    if(display)display_nullables();
    get_firsts();
    if(display)display_first();
    get_follows();
    if(display)display_follow();
    setup_parse_table();
    if(display)display_parse_table();

  }

  pair<bool,int> parse(vector<string> v){
    v.push_back(eol);
    int n=v.size();
    stack<string> st;
    st.push(eol);
    st.push(start_symbol);
    int cur_index=0;
    bool ok=true;
    while(st.size() and cur_index<n){
      auto val=st.top();
      st.pop();
      if(val==eps){
        continue;
      }
      if(val==eol){
        if(val!=v[cur_index]){
          ok=false;
        }
        break;
      }

      if(terminals.count(val)){//match
        if(val==v[cur_index]){
          cur_index++;
        }else{
          ok=false;
          break;
        }
      }else{//apply rule
        if(!parse_table[val].count(v[cur_index])){//rule not found
          //scan pop time
          if(v[cur_index]==eol or follows[val].count(v[cur_index])){//pop
            cout<<"popping "<<st.top()<<endl;
            st.pop();
          }else{
            cout<<"scanning"<<v[cur_index]<<endl;
            cur_index++;
          }
          continue;
          ok=false;
          break;
        }else{//insert the production here
          auto& temp_v=rules[val][parse_table[val][v[cur_index]]];
          for(auto it=temp_v.rbegin();it!=temp_v.rend();++it){
            st.push(*it);
          }
        }
      }
    }

    return {ok,cur_index};
  }

  vector<string> tokenize(const string& s){
    stringstream ss(s);
    string temp;
    vector<string> tokens;
    while(ss>>temp){
      tokens.push_back(temp);
    }
    return tokens;
  }

  void read_rules_from_file(const string& rule_file,const string& terminal_file){
    fstream rule_file_stream(rule_file);
    fstream terminal_stream(terminal_file);
    string s;

    while(getline(rule_file_stream,s)){
      if(s.empty()){
        continue;
      }
      auto vec=tokenize(s);
      if(start_symbol.empty()){
        start_symbol=vec[0];
      }
      if(vec.size()==1){
        rules[vec[0]].push_back({eps});
      }
      rules[vec[0]].push_back(vector<string>(vec.begin()+1,vec.end()));
    }
    s.clear();
    while(terminal_stream>>s){
      terminals.insert(s);
    }
    terminals.insert(eps);
    terminals.insert(eol);
  }

  void get_nullables(){
    while(true){
      int pre_nullable_count=nullables.size();
      for(const auto& t:rules){
        auto key=t.first;
        if(nullables.count(key)){//already in nullables
          continue;
        }
        for(const auto& v:t.second){
          if(nullables.count(key)){//added to nullables
            break;
          }
          int count=0;
          if(v.size()==1 and v[0]==eps){
            nullables.insert(key);
          }else{
            for(auto& x:v){
              count+=nullables.count(x);
            }
            if(count==v.size()){//all the terms are nullable
              nullables.insert(key);
            }
          }
        }
      }
      int post_nullable_count=nullables.size();
      if(pre_nullable_count==post_nullable_count){
        break;
      }
    }
  }
  
  static void __set_union(set<string>& a,const set<string>& b){
    for(const auto& x:b){
      a.insert(x);
    }
  }

  void get_firsts(){
    map<string,set<string>> edges;
    //setting up the edges;
    for(const auto& t:rules){
      auto key=t.first;
      for(const auto& v: t.second){
        for(auto& x:v){
          edges[key].insert(x);
          if(!nullables.count(x)){
            break;
          }
        }
      }
    }
    //graph traversal starting from all nodes;
    for(const auto& t:rules){
      auto start=t.first;
      set<string> visited;
      queue<string> q;
      q.push(start);
      visited.insert(start);
      while(q.size()){
        auto p=q.front();
        q.pop();
        if(terminals.count(p)){
          firsts[start].insert(p);
        }
        if(edges.count(p)){
          for(const auto& y:edges[p]){
            if(!visited.count(y)){
              q.push(y);
              visited.insert(y);
            }
          }
        }
        
      }
    }
    //applying the (first(y1)-eps)U(first(y2)) rule;
    for(auto& t:firsts){
      if(!nullables.count(t.first)){
        t.second.erase(eps);
      }else{
        t.second.insert(eps);//failsafe
      }
    }

    //add terminals too
    for(auto& t:terminals){
      firsts[t].insert(t);
    }
  }

  void get_follows(){

    //apply the simple solution
    map<string,set<string>> end_follows;

    //give special follow to the start_symbol
    follows[start_symbol].insert(eol);

    //get from the firsts of the rules
    for(const auto& p:rules){
      //read through the rules and calculate follow
      
      int rule_no=-1;
      for(const auto& v:p.second){
        rule_no++;
        set<string> accum_follows;
        int n=v.size();
        bool seen_non_null=false;
        for(int i=n-1;i>=0;--i){
          if(!seen_non_null){
            if(!terminals.count(v[i])){
              end_follows[v[i]].insert(p.first);//do the follow(v[i])=follow(S) thing
            }
          }
          if(!terminals.count(v[i])){//calc only for variables
            //make follow(v[i]) U= accum_follows
            __set_union(follows[v[i]],accum_follows);
            
          }

          if(!nullables.count(v[i])){
            seen_non_null=true;
            accum_follows.clear();
          }

          //accum_follows U= first(v[i])
          __set_union(accum_follows,firsts[v[i]]);

        }
      }
    }
    // display_follow();
    //get from the follows
    bool changed=true;
    while(changed){
      changed=false;//run until changes keep happening
      for(const auto& p:end_follows){
        for(const auto& t:p.second){
          int init_size=follows[p.first].size();
          __set_union(follows[p.first],follows[t]);
          if(init_size!=follows[p.first].size()){
            changed=true;
          }
        }
      }
    }


    //cut the epsilon from all follows;
    for(auto& p:follows){
      p.second.erase(eps);
    }

  }

  void __add_to_parse_table(const string& a,const set<string>& s,int index){
    for(const auto& b:s){
      if(parse_table[a].count(b)){
      //raise error
      throw not_ll1_exception(("cant insert rule:"+to_string(index)+". row:"+a+" col:"+b+" already has the rule:"+to_string(parse_table[a][b])));
      }
      parse_table[a][b]=index;
    }
    
  }
  
  void setup_parse_table(){
    try{
      for(const auto& p:rules){
        int index=0;
        for(const auto& v:p.second){
          
          if(v[0]==eps){//try follow of the symbol
            __add_to_parse_table(p.first,follows[p.first],index);
          }else{//keep checking the consecutive nullables;
            bool all_null=true;
            for(const auto& t:v){
              __add_to_parse_table(p.first,firsts[t],index);
              if(!nullables.count(t)){
                all_null=false;
                break;
              }
            }
            if(all_null){
              __add_to_parse_table(p.first,follows[p.first],index);
            }
          }
          ++index;
        }
      }
    }catch(not_ll1_exception& e){
      cerr<<"error happened"<<"\n";
      cerr<<e.what()<<endl;
      exit(-1);
    }

    
  }


};

const string Parser::eps="epsilon";
const string Parser::eol="$";



int main(int argc,char** argv){

  string rule_file(argv[1]);
  string terminal_file(argv[2]);
  string test_file(argv[3]);

  Parser p;
  p.setup_parser(rule_file,terminal_file,false);
  
  fstream fin(test_file,ios::in);
  int n=1;
  string s;
  // fin>>n;
  // fin.get();
  vector<vector<string>> vv;
  vector<vector<int>> word_nums;
  vector<vector<string>> actual_vals;

  vv.push_back({});
  word_nums.push_back({});
  actual_vals.push_back({});

  while(getline(fin,s)){
    if(s.empty()){
      continue;
    }
    vector<string> words;
    stringstream ss(s);
    while(ss>>s){
      words.push_back(s);
    }
    vv.back().push_back(words[0]);
    actual_vals.back().push_back(words[1]);
    // word_nums.back().push_back(stoi(words[-1]));
    //word number??
  }
  fin.close();

  boolalpha(cout);
  for(int i=0;i<n;++i){
    // cout<<"now parsing: "<<endl;
    // for(auto x:vv[i]){
    //   cout<<x<<"@";
    // }
    // cout<<endl;
    pair<bool,int> ans=p.parse(vv[i]);
    cout<<"stmt_"<<i<<" correct parsing status: "<<ans.first<<endl;
    if(!ans.first){
      cout<<"error occurred at:"<<actual_vals[i][ans.second]<<endl;
    }
  }


  return 0;
}