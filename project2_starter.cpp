#include <iostream>
#include <string>
#include <fstream>
#include <vector>

class Value {

    private:
    std::vector<int> stocks;
    std::vector<int> prices;
    std::vector<int> correct;
    int temp;

    public:
    //adds new input to the end of the array
    void input(int stock, int price){
        stocks.push_back(stock);
        prices.push_back(price);
    }

    //checks the total cost is under the max then returns the total stocks
    int calculate(int max){
        temp = 0;
        for (int i = 0; i < prices.size(); i++){
            temp += prices[i];
        }
        if (temp <= max){
            temp = 0;
                for (int i = 0; i < stocks.size(); i++){
                    temp += stocks[i];
                }
            return temp;
        }
        return -1;
    }

    //emptys old values
    void remove(){
        stocks.clear();
        prices.clear();
    }

    //formats the vectors and returns them for output
    std::string retrieve(int max){
        char part[33];
        itoa(calculate(max),part,10);
        std::string retrieval = "";
        std::string indexes;
        retrieval += part;
        retrieval += " # ";
        for(int i = 0; i < stocks.size(); i++){
            itoa(stocks[i],part,10);
            //std::cout << prices[i] << "\n";
            retrieval += part;
            if (i != (stocks.size() - 1)){
                retrieval += "+";
            }
            itoa(i,part,10);
            indexes += part;
            indexes += ",";
        }
        retrieval += " at index " + indexes + " sum of the values at these indecies = ";
        for (int i = 0; i < prices.size(); i++){
            itoa(prices[i],part,10);
            retrieval += part;
            if (i != (prices.size() - 1)){
                retrieval += "+";
            }
        }
        itoa(max,part,10);
        retrieval += "<=";
        retrieval += part;
        //std::cout << "\n\nseperating sections\n\n";
        return retrieval;
    }


    //trying to interpret slide code
    int knapsack(int maximum){
        int why = 1 + stocks.size();
        int mainarray[why][maximum + 1];
        //std::cout << stocks.size() << " " << maximum << "\n";
        for (int i = 0; i <= stocks.size(); i++){
            for (int j = 0; j <= maximum; j++){
                    mainarray[i][j] = 0;
                if(i > 0){
                    if(j - prices[i-1] >= 0){
                        if (mainarray[i-1][j] < (mainarray[i-1][j-prices[i-1]] + stocks[i-1])){
                            mainarray[i][j] = (mainarray[i-1][j-prices[i-1]] + stocks[i-1]);
                        } else {
                            mainarray[i][j] = mainarray[i-1][j];
                        }
                    } else {
                        mainarray[i][j] = mainarray[i-1][j];
                    }
                }
                //std::cout << mainarray[i][j] << " ";
            }
            //std::cout << "\n";
        }
        //std::cout << "seperate inputs\n";
            
            return mainarray[stocks.size()][maximum];
    }


};

int main(){
    
    //Declaring variables
    std::string output1 = "";
    std::string output2 = "Seperating Solution A output from Solution B output\n\n";
    std::string input;
    std::string conversion;
    int input_stock = 0;
    int input_price = 0;
    int current_length = 0;
    int best_spot = 0;
    int max_price;
    int dynamic_value;
    

    //Declaring class
    Value first;
    Value total;
    std::vector<Value> values;

    //Opening file to start retrieving values
    std::ifstream infile;
    infile.open("Input.txt");

    while(infile>>input){
        conversion = input[0];
            if(conversion == "[" && input.size() != 1){
                //code to extract stock
                conversion = input[1];
                input_stock = stoi(conversion);
                if (input.size() == 4){
                    conversion = input[2];
                    input_stock = (input_stock * 10) + stoi(conversion);
                }
                infile>>input;
                conversion = input[0];
                input_price = stoi(conversion);
                conversion = input[2];
                if (input.size() == 4 || conversion == "]"){
                    conversion = input[1];
                    input_price = (input_price * 10) + stoi(conversion);
                }
                //creating one complete array for part b
                total.input(input_stock,input_price);
                current_length = values.size();
                //creating many specific arrays for part a
                if(current_length == 0){
                    first.input(input_stock,input_price);
                    values.push_back(first);
                } else {
                    for (int i = current_length; i < (current_length * 2); i++){
                        //call a function to add current values to a new duplicate of the vector appended to the original
                        values.push_back(values[i - current_length]);
                        values[i].input(input_stock, input_price);
                    }
                    first.input(input_stock,input_price);
                    values.push_back(first);
                }
                first.remove();
            }
        if (input == "Amount"){
            if(values.size() != 0){
                infile>>input;
                infile>>input;
                max_price = stoi(input);
                //computing part a
                for (int i = 0; i < values.size(); i++){
                    //checks if new set is better than previous best, saving its spot if so
                    if(values[i].calculate(max_price) > values[best_spot].calculate(max_price)){
                        best_spot = i;
                    }
                }
                //takes current info and outputs it if it was not selected by default
                if (values[best_spot].calculate(max_price) > 0){
                    output1 += values[best_spot].retrieve(max_price);
                    output1 += "\n\n";
                } else {
                    output1 += "There were no stock options that fit within the budget of ";
                    char temporary [33];
                    itoa(max_price,temporary,10);
                    output1 += temporary;
                    output1 += ".\n\n";
                }
                values.clear();

                //computing part b
                //calls the dynamic function to sort through all data
                dynamic_value = total.knapsack(max_price);
                std::cout << "\n" << dynamic_value;

                if (dynamic_value > 0){
                    output2 += "The dynamically found maximum stock was ";
                    char temporary [33];
                    itoa(dynamic_value,temporary,10);
                    output2 += temporary;
                    output2 += ".\n\n";
                } else {
                    output2 += "There were no stock options that fit within the budget of ";
                    char temporary [33];
                    itoa(max_price,temporary,10);
                    output2 += temporary;
                    output2 += ".\n\n";
                }
                total.remove();
                //correct.clear();
            } else {
                //emergency output if no inputs were found
                output1 += "There were no stock options to choose from.\n\n";
                output2 += "There were no stock options to choose from.\n\n";
            }
        }
    }

    //Closing input file and outputting results
    infile.close();
    std::ofstream outfile;
    outfile.open ("Output.txt");
    outfile << output1;
    outfile << output2;
    outfile.close();
    return 0;
}