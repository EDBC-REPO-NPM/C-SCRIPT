#ifndef ERROR_CSCRIPT
#define ERROR_CSCRIPT
 
namespace std {

    void Error( system_error& e ){
        cout << "Exception caught (system_error):" << endl;
        cout << "Error    :" << e.what() << endl;
        cout << "Code     :" << e.code().value() << endl;
        cout << "Message  :" << e.code().message() << endl;
        cout << "Category :" << e.code().category().name() << endl;
    }

};

#endif 