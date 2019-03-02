#define BOOST_PYTHON_STATIC_LIB
#include "GalileoWrapper.h"
namespace GalileoWrap {

    GalileoWrap::GalileoWrap()
    {
        sdk = new GalileoSDK::GalileoSDK();
    }

    GalileoWrap::~GalileoWrap()
    {
        sdk->~GalileoSDK();
        free(sdk);
    }

    py_callable GalileoWrap::ConnectCB = py_callable(boost::python::object());
    py_callable GalileoWrap::DisconnectCB = py_callable(boost::python::object());

    GalileoSDK::GALILEO_RETURN_CODE
        GalileoWrap::Connect(std::string targetID, bool auto_connect, int timeout,
            boost::python::object OnConnect, boost::python::object OnDisconnect) {
        ConnectCB = py_callable{ OnConnect };
        DisconnectCB = py_callable{ OnDisconnect };
        void(*OnConnectTmp)(GalileoSDK::GALILEO_RETURN_CODE, std::string id);
        void(*OnDisconnectTmp)(GalileoSDK::GALILEO_RETURN_CODE, std::string id);
        OnConnectTmp = NULL;
        OnDisconnectTmp = NULL;
        if (PyCallable_Check(OnConnect.ptr())) {
            OnConnectTmp = [](GalileoSDK::GALILEO_RETURN_CODE status, std::string id) {
                ConnectCB(status, id);
            };
        }
        else if (Py_None != OnConnect.ptr()) {
            PyErr_SetString(PyExc_TypeError, "OnConnect expected type function or None");
            boost::python::throw_error_already_set();
            return GalileoSDK::GALILEO_RETURN_CODE::INVALIDE_PARAMS;
        }
        if (PyCallable_Check(OnDisconnect.ptr())) {
            OnDisconnectTmp = [](GalileoSDK::GALILEO_RETURN_CODE status, std::string id) {
                DisconnectCB(status, id);
            };
        }
        else if (Py_None != OnDisconnect.ptr()) {
            PyErr_SetString(PyExc_TypeError, "OnDisconnect expected type function or None");
            boost::python::throw_error_already_set();
            return GalileoSDK::GALILEO_RETURN_CODE::INVALIDE_PARAMS;
        }
        return sdk->Connect(targetID, auto_connect, timeout, OnConnectTmp, OnDisconnectTmp);
    }

    ServersList GalileoWrap::GetServersOnline() {
        return sdk->GetServersOnline();
    }

    boost::python::object GalileoWrap::GetCurrentServer() {
        GalileoSDK::ServerInfo* currentServer = sdk->GetCurrentServer();
        if (currentServer == NULL)
            return boost::python::object();
        else
            return boost::python::object(currentServer);
    }

    GalileoSDK::GALILEO_RETURN_CODE GalileoWrap::PublishTest() {
        return sdk->PublishTest();
    }

    GalileoSDK::GALILEO_RETURN_CODE GalileoWrap::SendCMD(boost::python::list cmd) {
        std::vector<uint8_t> cmd_vec = to_std_vector<uint8_t>(cmd);
        return sdk->SendCMD(cmd_vec.data(), cmd_vec.size());
    }

    GalileoSDK::GALILEO_RETURN_CODE GalileoWrap::StartNav() {
        return sdk->StartNav();
    }

    GalileoSDK::GALILEO_RETURN_CODE GalileoWrap::StopNav() {
        return sdk->StopNav();
    }

    GalileoSDK::GALILEO_RETURN_CODE GalileoWrap::SetGoal(int goalIndex) {
        return sdk->SetGoal(goalIndex);
    }

    GalileoSDK::GALILEO_RETURN_CODE GalileoWrap::PauseGoal() {
        return sdk->PauseGoal();
    }

    GalileoSDK::GALILEO_RETURN_CODE GalileoWrap::ResumeGoal() {
        return sdk->ResumeGoal();
    }

    GalileoSDK::GALILEO_RETURN_CODE GalileoWrap::CancelGoal() {
        return sdk->CancelGoal();
    }

    GalileoSDK::GALILEO_RETURN_CODE GalileoWrap::InsertGoal(float x, float y) {
        return sdk->InsertGoal(x, y);
    }

    GalileoSDK::GALILEO_RETURN_CODE GalileoWrap::ResetGoal() {
        return sdk->ResetGoal();
    }

    GalileoSDK::GALILEO_RETURN_CODE GalileoWrap::SetSpeed(float vLinear, float vAngle) {
        return sdk->SetSpeed(vLinear, vAngle);
    }

    GalileoSDK::GALILEO_RETURN_CODE GalileoWrap::Shutdown() {
        return sdk->Shutdown();
    }

    GalileoSDK::GALILEO_RETURN_CODE GalileoWrap::SetAngle(uint8_t sign, uint8_t angle) {
        return sdk->SetAngle(sign, angle);
    }

    GalileoSDK::GALILEO_RETURN_CODE GalileoWrap::StartLoop() {
        return sdk->StartLoop();
    }

    GalileoSDK::GALILEO_RETURN_CODE GalileoWrap::StopLoop() {
        return sdk->StopLoop();
    }

    GalileoSDK::GALILEO_RETURN_CODE GalileoWrap::SetLoopWaitTime(uint8_t time) {
        return sdk->SetLoopWaitTime(time);
    }

    GalileoSDK::GALILEO_RETURN_CODE GalileoWrap::StartMapping() {
        return sdk->StartMapping();
    }

    GalileoSDK::GALILEO_RETURN_CODE GalileoWrap::StopMapping() {
        return sdk->StopMapping();
    }

    GalileoSDK::GALILEO_RETURN_CODE GalileoWrap::SaveMap() {
        return sdk->SaveMap();
    }

    GalileoSDK::GALILEO_RETURN_CODE GalileoWrap::UpdateMap() {
        return sdk->UpdateMap();
    }

    GalileoSDK::GALILEO_RETURN_CODE GalileoWrap::StartChargeLocal() {
        return sdk->StartChargeLocal();
    }

    GalileoSDK::GALILEO_RETURN_CODE GalileoWrap::StopChargeLocal() {
        return sdk->StopChargeLocal();
    }

    GalileoSDK::GALILEO_RETURN_CODE GalileoWrap::SaveChargeBasePosition() {
        return sdk->SaveChargeBasePosition();
    }

    GalileoSDK::GALILEO_RETURN_CODE GalileoWrap::StartCharge(float x, float y) {
        return sdk->StartCharge(x, y);
    }

    GalileoSDK::GALILEO_RETURN_CODE GalileoWrap::StopCharge() {
        return sdk->StopCharge();
    }

    GalileoSDK::GALILEO_RETURN_CODE GalileoWrap::MoveTo(float x, float y, uint8_t &goalNum) {
        return sdk->MoveTo(x, y, &goalNum);
    }

    GalileoSDK::GALILEO_RETURN_CODE GalileoWrap::GetGoalNum(uint8_t &goalNum) {
        return sdk->GetGoalNum(&goalNum);
    }

    boost::python::object GalileoWrap::GetCurrentStatus() {
        galileo_serial_server::GalileoStatus status;
        if (sdk->GetCurrentStatus(&status) == GalileoSDK::OK) {
            return boost::python::object(status);
        }
        else
            return boost::python::object();
    }

    void GalileoWrap::SetCurrentStatusCallback(boost::python::object callback) {
        /*ConnectCB = callback;
        if (!PyCallable_Check(callback)) {
            ConnectCB = NULL;
        }
        
        

        boost::python::call<void>(callback, 1,1,1);*/

        return sdk->SetCurrentStatusCallback(boost::python::extract<void(*)(GalileoSDK::GALILEO_RETURN_CODE, galileo_serial_server::GalileoStatus)>(callback));
    }

    void GalileoWrap::SetGoalReachedCallback(boost::python::object callback) {
        return sdk->SetGoalReachedCallback(boost::python::extract<void(*)(int, galileo_serial_server::GalileoStatus)>(callback));
    }

    GalileoSDK::GALILEO_RETURN_CODE GalileoWrap::WaitForGoal(int goalID) {
        return sdk->WaitForGoal(goalID);
    }
}