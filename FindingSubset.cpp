template<typename T>
void findSubsets(vector<T> container, int n)
{
    for (int i = 0; i < (1 << n); ++i) {
        cout << "{ ";
        for (int j = 0; j < n; ++j)
            if (i & (1 << j))
                cout << container[j] << ",";
        cout << " }" << endl;
    }
}

int main() {

    vector<char> data_vec = { 'A', 'B', 'C', 'D' };
    findSubsets(data_vec, 4);

    vector<int> data_vec_integer = { 1, 2, 3, 4 };
    findSubsets(data_vec_integer, 4);

    vector<string> data_vec_str = { "Mino", "Mino2", "Kmin", "Goat"};
    findSubsets(data_vec_str, 4);

    return 0;
}
