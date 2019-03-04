#ifndef __GALILEO_WRAPPER_H__
#define __GALILEO_WRAPPER_H__

#include <GalileoSDK/GalileoSDK.h>
#include <boost/python.hpp>

namespace GalileoWrap {
    
    typedef std::vector<GalileoSDK::ServerInfo> ServersList;

    class with_gil
    {
    public:
        with_gil() { state_ = PyGILState_Ensure(); }
        ~with_gil() { PyGILState_Release(state_); }

        with_gil(const with_gil&) = delete;
        with_gil& operator=(const with_gil&) = delete;
    private:
        PyGILState_STATE state_;
    };

    class py_callable
    {
    public:

        /// @brief Constructor that assumes the caller has the GIL locked.
        py_callable(const boost::python::object& object)
        {
            with_gil gil;
            object_.reset(
                // GIL locked, so it is safe to copy.
                new boost::python::object{ object },
                // Use a custom deleter to hold GIL when the object is deleted.
                [](boost::python::object* object)
            {
                with_gil gil;
                delete object;
            });
        }

        // Use default copy-constructor and assignment-operator.
        py_callable(const py_callable&) = default;
        py_callable& operator=(const py_callable&) = default;

        template <typename ...Args>
        void operator()(Args... args)
        {
            // Lock the GIL as the python object is going to be invoked.
            with_gil gil;
            (*object_)(std::forward<Args>(args)...);
        }

    private:
        std::shared_ptr<boost::python::object> object_;
    };

    namespace py = boost::python;

    template< typename T >
    inline
        std::vector< T > to_std_vector(const py::object& iterable)
    {
        return std::vector< T >(py::stl_input_iterator< T >(iterable),
            py::stl_input_iterator< T >());
    }

    class GalileoWrap
    {
    public:
        GalileoWrap();
        GalileoSDK::GALILEO_RETURN_CODE
            Connect(std::string targetID, bool auto_connect, int timeout,
                boost::python::object OnConnect,
                boost::python::object OnDisconnect
            );
        ServersList GetServersOnline();
        boost::python::object GetCurrentServer();
        GalileoSDK::GALILEO_RETURN_CODE PublishTest();
        GalileoSDK::GALILEO_RETURN_CODE SendCMD(boost::python::list cmd);
        GalileoSDK::GALILEO_RETURN_CODE StartNav();
        GalileoSDK::GALILEO_RETURN_CODE StopNav();
        GalileoSDK::GALILEO_RETURN_CODE SetGoal(int goalIndex);
        GalileoSDK::GALILEO_RETURN_CODE PauseGoal();
        GalileoSDK::GALILEO_RETURN_CODE ResumeGoal();
        GalileoSDK::GALILEO_RETURN_CODE CancelGoal();
        GalileoSDK::GALILEO_RETURN_CODE InsertGoal(float x, float y);
        GalileoSDK::GALILEO_RETURN_CODE ResetGoal();
        GalileoSDK::GALILEO_RETURN_CODE SetSpeed(float vLinear, float vAngle);
        GalileoSDK::GALILEO_RETURN_CODE Shutdown();
        GalileoSDK::GALILEO_RETURN_CODE SetAngle(uint8_t sign, uint8_t angle);
        GalileoSDK::GALILEO_RETURN_CODE StartLoop();
        GalileoSDK::GALILEO_RETURN_CODE StopLoop();
        GalileoSDK::GALILEO_RETURN_CODE SetLoopWaitTime(uint8_t time);
        GalileoSDK::GALILEO_RETURN_CODE StartMapping();
        GalileoSDK::GALILEO_RETURN_CODE StopMapping();
        GalileoSDK::GALILEO_RETURN_CODE SaveMap();
        GalileoSDK::GALILEO_RETURN_CODE UpdateMap();
        GalileoSDK::GALILEO_RETURN_CODE StartChargeLocal();
        GalileoSDK::GALILEO_RETURN_CODE StopChargeLocal();
        GalileoSDK::GALILEO_RETURN_CODE SaveChargeBasePosition();
        GalileoSDK::GALILEO_RETURN_CODE StartCharge(float x, float y);
        GalileoSDK::GALILEO_RETURN_CODE StopCharge();
        GalileoSDK::GALILEO_RETURN_CODE MoveTo(float x, float y, uint8_t &goalNum);
        GalileoSDK::GALILEO_RETURN_CODE GetGoalNum(uint8_t &goalNum);
        boost::python::object GetCurrentStatus();
        void SetCurrentStatusCallback(boost::python::object callback);
        void SetGoalReachedCallback(boost::python::object callback);
        GalileoSDK::GALILEO_RETURN_CODE WaitForGoal(int goalID);
        ~GalileoWrap();
        static py_callable ConnectCB;
        static py_callable DisconnectCB;
        static py_callable UpdateStatusCB;
        static py_callable GoalReachedCB;
    private:
        GalileoSDK::GalileoSDK* sdk;
    };
}

#endif