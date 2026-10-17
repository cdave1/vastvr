#ifndef _VAST_LINKED_LIST_H_
#define _VAST_LINKED_LIST_H_ 

#include "core/collections/CollectionSettings.h"

/**
 * Implementation of an intrusive linked list as described here:
 *
 * www.codeofhonor.com/blog/avoiding-game-crashes-related-to-linked-list
 */

#define LIST_DECLARE(T, link) hd::ListDeclare<T, offsetof(T, link)>

// Define a field within a structure that will be used to link it into a list
#define LIST_LINK(T) hd::Link<T>

// Define a pointer to a list
#define LIST_PTR(T) hd::List<T> *

namespace hd
{
    template<class T>
    class Link
    {
    public:
        ~Link ();
        Link ();
        
        bool IsLinked () const;
        void Unlink ();
        
        T * Prev ();
        T * Next ();
        const T * Prev () const;
        const T * Next () const;
        
        // For use by list-type classes, not user code;
        // the alternative is to friend LinkedList<T>, THash<T>,
        // and (eventually) many other structures.
        Link (size_t offset);
        void SetOffset (size_t offset);
        Link<T> * NexLink ();
        Link<T> * PrevLink ();
        void InsertBefore (T * node, Link<T> * nexLink);
        void InsertAfter (T * node, Link<T> * prevLink);
        
    private:
        T *         m_nextNode; // pointer to the next >object<
        Link<T> *  m_prevLink; // pointer to the previous >link field<
        void RemoveFromList ();
        
        // Hide copy-constructor and assignment operator
        //Link (const Link &);
        Link & operator= (const Link &);
    };
    
    
    template<class T>
    Link<T>::~Link ()
    {
        RemoveFromList();
    }
    
    
    template<class T>
    Link<T>::Link ()
    {
        // Mark this node as the end of the list, with no link offset
        m_nextNode = (T *) ((size_t) this + 1 - 0);
        m_prevLink = this;
    }
    
    
    template<class T>
    Link<T>::Link (size_t offset)
    {
        // Mark this node as the end of the list, with the link offset set
        m_nextNode = (T *) ((size_t) this + 1 - offset);
        m_prevLink = this;
    }
    
    
    template<class T>
    void Link<T>::SetOffset (size_t offset)
    {
        // Mark this node as the end of the list, with the link offset set
        m_nextNode = (T *) ((size_t) this + 1 - offset);
        m_prevLink = this;
    }
    
    
    template<class T>
    Link<T> * Link<T>::NexLink ()
    {
        // Calculate the offset from a node pointer to a link structure
        size_t offset = (size_t) this - ((size_t) m_prevLink->m_nextNode & ~1);
        
        // Get the link field for the next node
        return (Link<T> *) (((size_t) m_nextNode & ~1) + offset);
    }
    
    
    template<class T>
    void Link<T>::RemoveFromList ()
    {
        NexLink()->m_prevLink = m_prevLink;
        m_prevLink->m_nextNode = m_nextNode;
    }
    
    
    template<class T>
    void Link<T>::InsertBefore (T * node, Link<T> * nexLink)
    {
        RemoveFromList();
        
        m_prevLink = nexLink->m_prevLink;
        m_nextNode = m_prevLink->m_nextNode;
        
        nexLink->m_prevLink->m_nextNode = node;
        nexLink->m_prevLink = this;
    }
    
    
    template<class T>
    void Link<T>::InsertAfter (T * node, Link<T> * prevLink)
    {
        RemoveFromList();
        
        m_prevLink = prevLink;
        m_nextNode = prevLink->m_nextNode;
        
        prevLink->NexLink()->m_prevLink = this;
        prevLink->m_nextNode = node;
    }
    
    
    template<class T>
    bool Link<T>::IsLinked () const
    {
        return m_prevLink != this;
    }
    
    
    template<class T>
    void Link<T>::Unlink ()
    {
        RemoveFromList();
        
        // Mark this node as the end of the list with no link offset
        m_nextNode = (T *) ((size_t) this + 1);
        m_prevLink = this;
    }
    
    
    template<class T>
    Link<T> * Link<T>::PrevLink ()
    {
        return m_prevLink;
    }
    
    
    template<class T>
    T * Link<T>::Prev ()
    {
        T * prevNode = m_prevLink->m_prevLink->m_nextNode;
        if ((size_t) prevNode & 1)
            return NULL;
        return prevNode;
    }
    
    
    template<class T>
    const T * Link<T>::Prev () const
    {
        const T * prevNode = m_prevLink->m_prevLink->m_nextNode;
        if ((size_t) prevNode & 1)
            return NULL;
        return prevNode;
    }
    
    
    template<class T>
    T * Link<T>::Next ()
    {
        if ((size_t) m_nextNode & 1)
            return NULL;
        return m_nextNode;
    }
    
    
    template<class T>
    const T * Link<T>::Next () const
    {
        if ((size_t) m_nextNode & 1)
            return NULL;
        return m_nextNode;
    }
    
    
    /******************************************************************************
     *
     *   LinkedList
     *
     ***/
    
    
    template<class T>
    class LinkedList
    {
    public:
        ~LinkedList ();
        LinkedList ();
        
        bool Empty () const;
        void UnlinkAll ();
        void DeleteAll ();
        
        T * Head ();
        T * Tail ();
        const T * Head () const;
        const T * Tail () const;
        
        T * Prev (T * node);
        T * Next (T * node);
        const T * Prev (const T * node) const;
        const T * Next (const T * node) const;
        
        void InsertHead (T * node);
        void InsertTail (T * node);
        void InsertBefore (T * node, T * before);
        void InsertAfter (T * node, T * after);
        
    private:
        Link<T>    m_link;
        size_t      m_offset;
        
        LinkedList (size_t offset);
        Link<T> * GeLinkFromNode (const T * node) const;
        //template<class T, size_t offset> friend class LinkedListDeclare;
        
        // Hide copy-constructor and assignment operator
        //LinkedList (const LinkedList &);
        LinkedList & operator= (const LinkedList &);
    };
    
    
    template<class T>
    LinkedList<T>::~LinkedList ()
    {
        UnlinkAll();
    }
    
    
    template<class T>
    LinkedList<T>::LinkedList () :
    m_link(),
    m_offset((size_t) -1)
    {}
    
    
    template<class T>
    LinkedList<T>::LinkedList (size_t offset) :
    m_link(offset),
    m_offset(offset)
    {}
    
    
    template<class T>
    bool LinkedList<T>::Empty () const
    {
        return m_link.Next() == NULL;
    }
    
    
    template<class T>
    void LinkedList<T>::UnlinkAll ()
    {
        for (;;) {
            Link<T> * link = m_link.PrevLink();
            if (link == &m_link)
                break;
            link->Unlink();
        }
    }
    
    
    template<class T>
    void LinkedList<T>::DeleteAll ()
    {
        while (T * node = m_link.Next())
            delete node;
    }
    
    
    template<class T>
    T * LinkedList<T>::Head ()
    {
        return m_link.Next();
    }
    
    
    template<class T>
    T * LinkedList<T>::Tail ()
    {
        return m_link.Prev();
    }
    
    
    template<class T>
    const T * LinkedList<T>::Head () const
    {
        return m_link.Next();
    }
    
    
    template<class T>
    const T * LinkedList<T>::Tail () const
    {
        return m_link.Prev();
    }
    
    
    template<class T>
    T * LinkedList<T>::Prev (T * node)
    {
        return GeLinkFromNode(node)->Prev();
    }
    
    
    template<class T>
    const T * LinkedList<T>::Prev (const T * node) const
    {
        return GeLinkFromNode(node)->Prev();
    }
    
    
    template<class T>
    T * LinkedList<T>::Next (T * node)
    {
        return GeLinkFromNode(node)->Next();
    }
    
    
    template<class T>
    const T * LinkedList<T>::Next (const T * node) const
    {
        return GeLinkFromNode(node)->Next();
    }
    
    
    template<class T>
    void LinkedList<T>::InsertHead (T * node)
    {
        InsertAfter(node, NULL);
    }
    
    
    template<class T>
    void LinkedList<T>::InsertTail (T * node)
    {
        InsertBefore(node, NULL);
    }
    
    
    template<class T>
    void LinkedList<T>::InsertBefore (T * node, T * before)
    {
        hdAssert(!((size_t) node & 1));
        GeLinkFromNode(node)->InsertBefore(
                                           node,
                                           before ? GeLinkFromNode(before) : &m_link
                                           );
    }
    
    
    template<class T>
    void LinkedList<T>::InsertAfter (T * node, T * after)
    {
        hdAssert(!((size_t) node & 1));
        GeLinkFromNode(node)->InsertAfter(
                                          node,
                                          after ? GeLinkFromNode(after) : &m_link
                                          );
    }
    
    
    template<class T>
    Link<T> * LinkedList<T>::GeLinkFromNode (const T * node) const
    {
        hdAssert(m_offset != (size_t) -1);
        return (Link<T> *) ((size_t) node + m_offset);
    }
    
    
    /**
     * LinkedListDeclare - declare a list with a known link offset
     */
    
    template<class T, size_t offset>
    class LinkedListDeclare : public LinkedList<T>
    {
    public:
        LinkedListDeclare ();
    };
    
    
    template<class T, size_t offset>
    LinkedListDeclare<T, offset>::LinkedListDeclare () : LinkedList<T>(offset)
    {}
}


#endif