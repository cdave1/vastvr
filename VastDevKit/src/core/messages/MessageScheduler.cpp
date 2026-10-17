#include "MessageScheduler.h"

hd::MessageScheduler *g_defaultMessageScheduler = NULL;


#pragma mark -
#pragma mark Public functions

hd::MessageScheduler * hd::MessageScheduler::DefaultEventScheduler() {
    if (g_defaultMessageScheduler == NULL) {
        g_defaultMessageScheduler = new MessageScheduler();
    }
    return g_defaultMessageScheduler;
}


void hd::MessageScheduler::StopReceiver(hd::TimedMessenger *event) {
    if (event == NULL) return;
    MessageScheduler::DefaultEventScheduler()->StopReceiverInternal(event);
}


void hd::MessageScheduler::ScheduleReceiver(hd::TimedMessenger *handler, const float delay) {
    handler->SetDelay(delay);
    //handler->AddCallback<T>(hdScheduledEventCallbackName, obj, callbackFunc);
    MessageScheduler::DefaultEventScheduler()->StartReceiver(handler);
}


void hd::MessageScheduler::StartReceiver(hd::TimedMessenger *receiver) {
    assert (!IsLocked());
    
    if (receiver->IsRunning()) {
        hd::log::e("Event already running.");
    } else {
        receiver->Start();
        
        pthread_mutex_lock(&m_mutex);
        m_currentReceivers.push_back(receiver);
        pthread_mutex_unlock(&m_mutex);
    }
}


void hd::MessageScheduler::Tick(const hdTimeInterval interval) {
    if (m_currentReceivers.empty()) {
        return;
    }
    
    assert (!IsLocked());
    
    pthread_mutex_lock(&m_mutex);
    int currEventCount = m_currentReceivers.size(), currIdx = 0;
    TimedMessenger * currentEventsArr[currEventCount];
    std::list<TimedMessenger *>::iterator it;
    for (it = m_currentReceivers.begin(); it != m_currentReceivers.end(); ++it) {
        currentEventsArr[currIdx++] = (*it);
    }
    pthread_mutex_unlock(&m_mutex);
    
    std::list<TimedMessenger *> toRemove;
    for (int i = 0; i < currEventCount; ++i) {
        TimedMessenger * receiver = currentEventsArr[i];
        receiver->Tick(interval);
        if (receiver->IsFinished()) {
            toRemove.push_back(receiver);
        }
    }
    
    int removeCount = toRemove.size();
    TimedMessenger * removed[removeCount];
    currIdx = 0;
    
    for (it = toRemove.begin(); it != toRemove.end(); ++it) {
        pthread_mutex_lock(&m_mutex);
        TimedMessenger *event = (*it);
        removed[currIdx++] = event;
        m_currentReceivers.remove(event);
        pthread_mutex_unlock(&m_mutex);
    }
    
    for (int i = 0; i < removeCount; ++i) {
        TimedMessenger * receiver = removed[i];
        receiver->Finish();
    }
}


#pragma mark -
#pragma mark Private funcs

hd::MessageScheduler::MessageScheduler() {
    pthread_mutex_init(&m_mutex, NULL);
}


hd::MessageScheduler::~MessageScheduler() {
    pthread_mutex_destroy(&m_mutex);
}


void hd::MessageScheduler::StopReceiverInternal(TimedMessenger * receiver) {
    assert (!IsLocked());
    assert (receiver != NULL);
    if (receiver->IsRunning()) {
        receiver->Stop();
        pthread_mutex_lock(&m_mutex);
        m_currentReceivers.remove(receiver);
        pthread_mutex_unlock(&m_mutex);
    }
}


bool hd::MessageScheduler::IsLocked() {
    if (0 != pthread_mutex_trylock(&m_mutex)) {
        return true;
    }
    pthread_mutex_unlock(&m_mutex);
    return false;
}
