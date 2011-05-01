//
//  lr_state.h
//  Parse
//
//  Created by Andrew Hunter on 01/05/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef _LR_LR_STATE_H
#define _LR_LR_STATE_H

#include <set>
#include "Util/container.h"
#include "Lr/lr_item.h"

namespace lr {
    ///
    /// \brief Functor used in cases where LR states do not need to be merged
    ///
    template<typename item_type> class state_no_merge {
        inline bool operator()(item_type& mergeInto, const item_type& mergeFrom) {
            return false;
        }
    };
    
    ///
    /// \brief Representation of a generic LR state
    ///
    /// item_type represents the type of the LR item (for example, lr0_item)
    /// compare_items represents how an item is compared
    /// merge_items represents how an item is merged with an identical item (a merge operation must not affect the item ordering)
    ///
    template<typename item_type, typename compare_items = std::less<item_type>, typename merge_items = state_no_merge<item_type> > class lr_state {
    public:
        /// \brief Container for an item
        typedef util::container<item_type> container;
        
        /// \brief Item container comparator type
        typedef container::compare_adapter<compare_items> comparator;
        
        /// \brief Set of items within a state
        typedef std::set<container, comparator> item_set;
        
        /// \brief Iterator that can be used to retrieve the contents of this state
        typedef typename item_set::const_iterator iterator;
        
    private:
        /// \brief The items making up this state
        item_set m_Items;
        
    public:
        /// \brief Constructs an empty state
        lr_state() { }
        
        lr_state(const lr_state& copyFrom)
        : m_Items(copyFrom.m_Items) {
        }
        
    public:
        /// \brief Equality operator
        bool operator==(const lr_state& compareTo) const {
            if (m_Items.size() != compareTo.m_Items.size()) return false;
            
            static comparator less_than;
            
            for (iterator ours = m_Items.begin(), theirs = compareTo.begin(); ours != m_Items.end() && theirs != compareTo.m_Items.end(); ours++, theirs++) {
                // Not equal if either greater than or less that this item,
                if (less_than(*ours, *theirs) || less_than(*theirs, *ours)) return false;
            }
            
            return true;
        }
        
        /// \brief Ordering operator
        bool operator<(const lr_state& compareTo) const { 
            if (m_Items.size() < compareTo.m_Items.size()) return true;
            if (m_Items.size() > compareTo.m_Items.size()) return false;

            static comparator less_than;

            for (iterator ours = m_Items.begin(), theirs = compareTo.begin(); ours != m_Items.end() && theirs != compareTo.m_Items.end(); ours++, theirs++) {
                // Not equal if either greater than or less that this item,
                if (less_than(*ours, *theirs)) return true;
                if (less_than(*theirs, *ours)) return false;
            }
            
            // Equal if we reach here
            return false;
        }

        inline bool operator!=(const lr_state& compareTo) const { return !operator==(compareTo); }
        inline bool operator>(const lr_state& compareTo) const { return !operator<(compareTo) && !operator==(compareTo); }
        inline bool operator<=(const lr_state& compareTo) const { return operator<(compareTo) || operator==(compareTo); }
        inline bool operator>=(const lr_state& compareTo) const { return !operator<(compareTo); }
        
    public:
        /// \brief Clones this state
        lr_state* clone() { return new lr_state(*this); }
        
        /// \brief Comparison function
        static inline bool compare(const lr_state& a, const lr_state& b) { return a < b; }

        /// \brief Comparison function
        static inline bool compare(const lr_state* a, const lr_state* b) { 
            if (a == b) return false;
            if (!a) return true;
            if (!b) return false;
            
            return a < b;
        }
        
    public:
        /// \brief Adds a new item to this object. Returns true if the operation modified this container
        bool add(const container& newItem) {
            // Try to find an existing example of this item
            typename item_set::iterator found = m_Items.find(newItem);
            
            static merge_items do_merge;
            
            if (found == m_Items.end()) {
                // Just add the item if it wasn't found
                m_Items.insert(newItem);
                return true;
            } else {
                // Merge the items if it already exists
                item_type merged = **found;
                
                // Replace the old item if the merge was successful
                if (do_merge(merged, *newItem)) {
                    m_Items.erase(found);
                    m_Items.insert(merged);
                    return true;
                } else {
                    return false;
                }
            }
        }

    public:
        /// \brief The first item in this state
        inline iterator begin() const { return m_Items.begin(); }
        
        /// \brief The final item in this state
        inline iterator end() const { return m_Items.end(); }
    };
}

#endif
