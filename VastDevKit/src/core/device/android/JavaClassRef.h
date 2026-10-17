
#ifndef _JAVA_CLASS_REF_H_
#define _JAVA_CLASS_REF_H_

#include <jni.h>

class JavaClassRef
{
private:
    JavaVM * m_jvm;
    jobject m_ref;
    
public:
    
    JavaClassRef(JavaVM * jvm);
    
    ~JavaClassRef();
    
    void SetJVM(JavaVM * jvm);
    
    JavaVM * GetJVM() const;
    
    JNIEnv * GetEnv() const;
    
    void SetRef(jobject ref);
    
    jobject GetRef() const;
    

    
};

#endif