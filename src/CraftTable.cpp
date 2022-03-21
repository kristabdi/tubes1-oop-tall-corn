#include <iostream>
#include "CraftTable.hpp"

using namespace std;

string get_cid(int idx) {
    // Get c_id from a index
    char num = idx + '0';
    string res = "C";
    res.push_back(num);
    return res;
};

int get_idx(string c_id) {
    int res = c_id[1] - '0';
    return res;
};

bool isCIDValid(string c_id) {
    int idx = get_idx(c_id);
    return (idx >= 0 && idx < MAX_CAP);
};


bool isSubArray(vector<string> A, vector<string> B, int n, int m) {
    // to check if B is subarray of A
    int i = 0, j = 0;
    while (i < n && j < m) {
        if (A[i] == B[j]) {
            i++;
            j++;
            if (j == m)
                return true;
        }
        else {
            i = i - j + 1;
            j = 0;
        }
    }
    return false;
}

vector<string> trimKosong(vector<string> table) {
    for (int i = 0; i < table.size(); i++) {
        if (table[i] == "-") {
            table.erase(table.begin()+i);
        } else {
            break;
        }
    }

    for (int i = table.size()-1; i>=0; i++) {
        if (table[i] == "-") {
            table.pop_back();
        } else {
            break;
        }
    }

    return table;
};

vector<string> reflectYTable(vector<string> table) {
    swap(table[0], table[2]);
    swap(table[3], table[5]);
    swap(table[6], table[8]);
    return table;
}


// Ctor
CraftTable::CraftTable() {
    for (int i = 0; i < MAX_CAP; i++) {
        string key = get_cid(i);
        table[key] = NULL;
    }
}; 
// CCtor
CraftTable::CraftTable(const CraftTable& other) {
    table = other.table;
};

// Overload
CraftTable& CraftTable::operator=(const CraftTable& other) {
    table = other.table;
    return *this;
};

// Getter Setter Push Pop
Item& CraftTable::getItemInCraftTable(string c_id) {
    if (isCIDValid(c_id))
        return *table[c_id];
    else {
        throw new CIDNotValid();
    }
};

void CraftTable::add(Item& item, string c_id) {
    cout << "Adding to " << c_id << "item named " << item.getName();
    // Add item to c_id slot, if not empty throw error
    if (isCIDValid(c_id)) {
        cout << "is slot empty "<<isSlotEmpty(c_id) << endl;
        if (isSlotEmpty(c_id)) {
            table[c_id] = &item;
            cout<<"isi table cid" << table[c_id]->getName() << endl;
        } else {
            cout << "THROW sini" << endl;
            throw new craftTableIsNotEmptyException();
        }
    } else {
        cout << "THROW" << endl;
        throw new CIDNotValid();
    }
}; 
void CraftTable::substract(string c_id) {
    // Delete item from c_id slot to be empty, if empty throw error
    if (isCIDValid(c_id)) {
        if (!isSlotEmpty(c_id)) {
            table[c_id] = NULL;
        } else {
            throw new craftTableIsEmptyException();
        }
    } else {
        throw new CIDNotValid();
    }
}; 

// Method
void CraftTable::print() {
   for (int i = 0; i < MAX_CAP; i++) {
        string key = get_cid(i);
        if (table[key]) {
            if (i == 2 || i == 5) {
                cout << table[key]->getName() << endl;
            } else {
                cout << table[key]->getName() << " ";
            }
        } else {
            if (i == 2 || i == 5) {
                cout << "KOSONG" << endl;
            } else {
                cout << "KOSONG" << " ";
            }
        }
    }
    cout << endl;
}; 

Item* CraftTable::make(vector<Recipe*> recipe) {
    /* Jika terdapat resep yang memenuhi, Item bahan akan hilang dan Item hasil akan muncul. Item akan otomatis ditambahkan ke inventory dengan
    algoritma yang sama dengan command GIVE. */
    if (!isTableEmpty()) {

        if (!this->isAllTool()) {
            bool reflected = false;
            int idx = whichBuildable(recipe, reflected);
            if (idx != -1) {
                // If recipe matched with craft table
                Recipe* itemCrafted = recipe[idx];
                int multiplicity = this->checkMultiple();
                return new NonTool(itemCrafted->getItem()->getID(), itemCrafted->getItem()->getName(), itemCrafted->getItem()->getNonToolClass(), itemCrafted->getQuantityResult());
            } else {
                // If recipe not matched with craft table, check craft table reflected by Y-axis
                reflected = true;
                idx = whichBuildable(recipe, reflected);
                if (idx != -1) {
                    int multiplicity = this->checkMultiple();
                    Recipe* itemCrafted = recipe[idx];
                    return new NonTool(itemCrafted->getItem()->getID(), itemCrafted->getItem()->getName(), itemCrafted->getItem()->getNonToolClass(), itemCrafted->getQuantityResult());
                } else {
                    // cannot build items in craft table
                    throw new craftTableDoesntMatchRecipeException();
                }
            }

        } else {
            // If build Tool sum by durability
            Tool* sumTool = makeTool();
            Item* res = new Tool(sumTool->getID(), sumTool->getName(), sumTool->getDurability());
            return res;
        }
        
    } else {
        throw new craftTableIsEmptyException;
    }
}; 

Tool* CraftTable::makeTool() {
    string name;
    int sum = 0, id = 0;
    for (auto it = table.begin(); it != table.end(); ++it) {
        if (it->second) {
            id = it->second->getID();
            name = it->second->getName();
            sum += it->second->getDurability();
        }
    }
    sum = min(10, sum);
    return new Tool(id, name, sum);
};

int CraftTable::whichBuildable(vector<Recipe*> listRecipe, bool reflected) {
    int res = -1;
    for (int i = 0; i < listRecipe.size(); i++) {
        int row = listRecipe[i]->getRow();
        int col = listRecipe[i]->getCol();
        vector<string> recipe = listRecipe[i]->getRecipe();
        vector<string> table;
        if (!reflected) {
            table = (this->convertVector());
        } else {
            table = reflectYTable(this->convertVector());
        }
        table = trimKosong(table);
        // check recipe is subarray of a table array
        if (isSubArray(table, recipe, table.size(), recipe.size())) {
            res = i;
            break;
        }
    }
    return res;
};

vector<string> CraftTable::convertVector() {
    vector<string> res;
    string val;
    for (auto it = table.begin(); it != table.end(); ++it) {
        if (it->second) {
            val = it->second->getName();
        } else {
            val = "-";
        }
        res.push_back(val);
    }
    return res;
}

// bool CraftTable::contain(Item& item) {
//     for (auto it = table.begin(); it != table.end(); ++it) {
//         if (it->second == &item) return true;
//     }
//     return false;
// }

// Attribute
bool CraftTable::isSlotEmpty(string c_id) {
    if (isCIDValid(c_id))
        return (!table[c_id]);
    return false;
};

bool CraftTable::isTableEmpty() {
    for (auto it = table.begin(); it != table.end(); ++it) {
        if (it->second) return false;
    }

    return true;
};

bool CraftTable::isAllTool() {
    int cnt = 2;
    for (auto it = table.begin(); it != table.end(); ++it) {
        if (it->second) {
            // if slot not empty
            if (it->second->getIsTool() && cnt >= 0) {
                --cnt;
            } else {
                //if (!it->second->getIsTool() || cnt < 0)                
                return false;
            }
        }
    } 
    return true;
};

int CraftTable::checkMultiple() {
    int idx = 0;
    int multiplicity = 0;
    for (auto it = table.begin(); it != table.end(); ++it) {
        if (it->second) {
            if (it->second->getQuantity() != 1) {
                int quantityItem = it->second->getQuantity();
                if (idx == 0) {
                    multiplicity = quantityItem;
                } else {
                    if (multiplicity > quantityItem) {
                        multiplicity = quantityItem;
                    }
                }

            } else {
                return 1;
            }
        }
        ++idx;
    }
    return multiplicity;
};


// int CraftTable::whichBuildableReflected(vector<Recipe*> listRecipe) {
//     int res = -1;
//     for (int i = 0; i < listRecipe.size(); i++) {
//         int row = listRecipe[i]->getRow();
//         int col = listRecipe[i]->getCol();
//         vector<string> recipe = listRecipe[i]->getRecipe();
//         vector<string> tableYReflected = reflectYTable(this->convertVector());
//         tableYReflected = trimKosong(tableYReflected);
//         // check recipe is subarray of a table array
//         if (isSubArray(tableYReflected, recipe, tableYReflected.size(), recipe.size())) {
//             res = i;
//             break;
//         }
//     }
//     return res;
// };