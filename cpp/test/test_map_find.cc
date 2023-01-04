#include <util/base.h>

#include <sparsehash/dense_hash_map>
#include <sparsehash/dense_hash_set>
#include <sparsehash/sparse_hash_map>
#include <sparsehash/sparse_hash_set>

int main() {
  std::vector<std::string> ori_test_data{
    "Scientists","at","Columbia","University","have","warned","that","rise","of","subvariants","such","as","XBB","family","could","result","in","a","surge","of","breakthrough","infections","as","well","as","re-infections",
    "XBB.1.5","is","highly","immune","evasive","appears","to","bind","better","to","cells","than","other","members","of","XBB","omicron","subvariant","family",
    "Covid","omicron","XBB.1.5","variant","is","rapidly","becoming","dominant","in","U.S.","because","it","is","highly","immune","evasive","appears","more","effective","at","binding","to","cells","than","related","subvariants,","scientists","say",
    "Scientists","public","health","officials","have","been","closely","monitoring","XBB","subvariant","family","for","months","because","strains","have","many","mutations","that","could","render","Covid-19","vaccines,","including","omicron","boosters,","less","effective","cause","even","more","breakthrough","infections",
    "XBB","subvariants","are","also","resistant","to","Evusheld,","an","antibody","cocktail","that","many","people","with","weak","immune","systems","rely","on","for","protection","against","Covid","infection","because","they","don’t","mount","a","strong","response","to","vaccines",
    "Dr.","Anthony","Fauci,","who","is","leaving","his","role","as","White","House","chief","medical","advisor,","has","previously","said","that","XBB","subvariants","reduce","protection","boosters","provide","against","infection","“multifold.”",
    "Fauci","said","he","was","encouraged","by","case","of","Singapore,","which","had","a","major","surge","of","infections","from","XBB","but","did","not","see","hospitalizations","rise","at","same","rate.","Pekosz","said","XBB.1.5,","in","combination","with","holiday","travel,","could","cause","cases","to","rise","in","U.S.","But","he","said","boosters","appear","to","be","preventing","severe","disease",
    "number","of","“economically","inactive”","people","—","those","neither","working","nor","looking","for","a","job","—","between","ages","of","16","64","has","risen","by","more","than","630,000","since","2019.","Unlike","other","major","economies,","recent","U.K.","data","shows","no","sign","that","these","lost","workers","are","returning","to","labor","market,","even","as","inflation","energy","costs","exert","huge","pressure","on","household","finances",
    "Younger","people","have","also","seen","some","of","largest","relative","increases,","some","industries","such","as","wholesale","retail","are","affected","to","a","greater","extent","than","others",
    "Though","effects","of","issues","mentioned","above","haven’t","been","quantified,","report","suggested","increase","has","been","driven","by","“other","health","problems","or","disabilities,”","“mental","illness","nervous","disorders,”","“problems","connected","with","[the]","back","or","neck.”",
    "Jonathan","Portes,","professor","of","economics","and","public","policy","at","King’s","College","London,","told","CNBC","the","scale","of","the","labor","market","depletion","is","likely","a","combination","of","long","Covid;","other","pandemic-related","health","issues","such","as","mental","illness;","and","the","current","crisis","in","the","NHS",
    "Although","the","pandemic","has","greatly","worsened","the","health","crisis","leaving","a","hole","in","the","U.K.","economy,","the","rise","in","long-term","sickness","claims","actually","began","in","2019,","and","economists","see","several","possible","reasons","why","the","country","has","been","uniquely","vulnerable",
    "In","the","aftermath","of","the","pandemic,","NHS","waiting","lists","grew","at","the","fastest","rate","since","records","began","in","August","2007,","a","recent","House","of","Commons","report","highlighted,","with","more","than","7","million","patients","on","the","waiting","list","for","consultant-led","hospital","treatment","in","England","as","of","September",
    "Former","Bank","of","England","policymaker","Michael","Saunders,","now","a","senior","policy","advisor","at","Oxford","Economics,","also","told","CNBC","that","the","U.K.","has","been","particularly","badly","affected","by","Covid","in","terms","of","severity,","and","that","some","of","this","may","have","been","the","result","of","the","country’s","higher","rates","of","preexisting","health","conditions","such","as","obesity","which","may","have","been","exacerbated","by","Covid",
    "Southwest","had","canceled","304","flights","since","Friday,","2%","of","its","schedule,","most","of","them","on","Monday","when","U.S.","airlines","faced","bad","weather","and","ground","stops","in","Florida","tied","to","a","Federal","Aviation","Administration","equipment","outage.","For","comparison,","from","Dec.","21","through","Dec.","29","Southwest","had","scrubbed","about","45%","of","its","operation,","a","far","bigger","share","than","other","major","airlines,","according","to","FlightAware",
    "Transportation","Secretary","Pete","Buttigieg","vowed","to","hold","Southwest","accountable","if","it","didn’t","provide","customers","with","refunds","and","reimbursements,","though","such","fines","associated","with","a","failure","to","pay","back","customers","can","take","months","if","not","years",
    "Manhattan","apartment","sales","fell","by","29%","in","the","fourth","quarter,","sparking","fears","of","a","frozen","market","in","which","buyers","and","sellers","stay","on","the","sidelines","due","to","economic","and","rate","fears",
    "Thedeclines","in","both","sales","and","prices","mark","the","end","of","the","roaring","comeback","in","Manhattan","real","estate","after","the","worst","days","of","the","pandemic","and","raise","fears","of","continuing","weakness","into","the","new","year.","Rising","interest","rates,","a","weaker","economy","and","a","falling","stock","market,","which","has","an","outsized","impact","on","Manhattan","real","estate,","are","all","likely","to","weigh","on","the","market","this","year",
    "Thepipeline","of","deals","in","the","works","or","recently","signed","suggests","a","slow","first","quarter.","There","were","only","2,312","contracts","signed","in","the","fourth","quarter,","down","43%","over","last","year,","according","to","Brown","Harris","Stevens.","The","quarter","was","the","worst","for","new","contracts","signed","in","the","past","decade,","according","to","a","report","from","Serhant",
    "Brokers,however,","say","they","remain","optimistic","and","many","are","predicting","an","upside","surprise","in","2023,","as","rates","stabilize","and","buyers","find","opportunities","in","a","softer","market.","John","Gomes,","co-founder","of","the","Eklund","Gomes","team","at","Douglas","Elliman,","said","December","was","“on","fire”","with","a","frenzy","of","year-end","deals",
  };
  std::vector<std::string> test_data;
  test_data.reserve(ori_test_data.size());
  for (auto& s : ori_test_data) {
    if (s.length() < 7) continue;
    test_data.push_back(s);
  }

  std::set<std::string> std_set;
  std::unordered_set<std::string> std_unordered_set;
  google::sparse_hash_set<std::string> google_spare_set; // (test_data.size());
  google::dense_hash_set<std::string> google_dense_set; // (test_data.size());
  google_spare_set.set_deleted_key("");
  google_dense_set.set_empty_key("");
  for (auto& s : test_data) {
    std_set.insert(s);
    std_unordered_set.insert(s);
    google_spare_set.insert(s);
    google_dense_set.insert(s);
  }
  std::cout << "size " << std_set.size() << std::endl;

  const int test_run_num = 10000000;
  std::vector<int> test_access_index(test_run_num, 0);
  std::default_random_engine rand_engine(nowUs());
  std::uniform_int_distribution<int> rand_dist(0, int(test_data.size()) - 1);
  for (int i = 0; i < test_run_num; i++) {
    test_access_index[i] = rand_dist(rand_engine);
  }

  #define DO_TEST(set_val) { \
    int64_t cost = nowMs(); \
    for (int idx : test_access_index) { \
      auto iter = set_val.find(test_data[idx]); \
      if (iter == set_val.end()) { \
        std::cout << ""#set_val" lost " << idx << " \"" << test_data[idx] << "\"" << std::endl; \
        std::abort(); \
      } \
    } \
    cost = nowMs() - cost; \
    std::cout << ""#set_val" cost " << cost << std::endl; \
  }
  DO_TEST(std_set);
  DO_TEST(std_unordered_set);
  DO_TEST(google_spare_set);
  DO_TEST(google_dense_set);
}
