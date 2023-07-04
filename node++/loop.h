#ifndef NODEPP_LOOP
#define NODEPP_LOOP
#include <mutex>

namespace LOOP_NODEPP {
    class NODE { public: function_t<int> data; 
        NODE* nxt = nullptr; NODE* prv = nullptr;
        NODE( function_t<int> func ){ data = func; }
    };
}

namespace process {

    std::mutex mut; int _act_= 0; 

    namespace loop {

        LOOP_NODEPP::NODE* queue = nullptr;

        bool is_active(){ return ( queue != nullptr ); }

        template< class T, class... V >
        void add( T _func, V... arg ){ 
            std::lock_guard<std::mutex> lock(mut);

            auto _func_ = [=](){ return _func( arg... ); };
            
            if( queue == nullptr ){
                queue = new LOOP_NODEPP::NODE(_func_);
            } else {
                auto n = queue; while( n->nxt != nullptr ){ n = n->nxt; }
                n->nxt = new LOOP_NODEPP::NODE(_func_);
                n->nxt->prv = n;
            }

        }

        void next(){
            static LOOP_NODEPP::NODE* n = nullptr;

            if( n == nullptr ){ n = queue; return; }
            int result = n->data();

            if (result < 0) {
                if( n == queue ) {
                    queue = n->nxt; delete n; n = queue;
                    if (n != nullptr) n->prv = nullptr;
                } else {
                    auto prv = n->prv;
                    prv->nxt = n->nxt;
                    if( n->nxt != nullptr )
                        n->nxt->prv = prv;
                    delete n; n = prv->nxt;
                }
            } else { n = n->nxt; }
        }

    }

    namespace poll {

        LOOP_NODEPP::NODE* queue = nullptr;

        bool is_active(){ return ( queue != nullptr ); }

        template< class T, class... V >
        void add( T _func, V... arg ){ 
            std::lock_guard<std::mutex> lock(mut);

            auto _func_ = [=](){ return _func( arg... ); };
            
            if( queue == nullptr ){
                queue = new LOOP_NODEPP::NODE(_func_);
            } else {
                auto n = queue; while( n->nxt != nullptr ){ n = n->nxt; }
                n->nxt = new LOOP_NODEPP::NODE(_func_);
                n->nxt->prv = n;
            }

        }

        void next(){
            static LOOP_NODEPP::NODE* n = nullptr;

            if( n == nullptr ){ n = queue; return; }
            int result = n->data();

            if( result < 0 ) {
                if( n == queue ) {
                    queue = n->nxt; delete n; n = queue;
                    if (n != nullptr) n->prv = nullptr;
                } else {
                    auto prv = n->prv;
                    prv->nxt = n->nxt;
                    if( n->nxt != nullptr )
                        n->nxt->prv = prv;
                    delete n; n = prv->nxt;
                }
            } else if( result == 0 ) { n = n->nxt; }
        }

    }

}
#endif