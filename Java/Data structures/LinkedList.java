import java.util.*;

public class LinkedList<T> implements List<T> {
	
	private Item<T> firstInList = null;
	
	private Item<T> lastInList = null;
	
	private int size;
	
	@Override
	public int size() {
		return this.size;
	}
	
	@Override
	public boolean isEmpty() {
		return this.size() == 0;
	}
	
	@Override
	public boolean contains(final Object o) {
		// BEGIN (write your solution here)
        return (indexOf(o) != -1);
		// END
	}
	
	@Override
	public Iterator<T> iterator() {
		return new ElementsIterator(0);
	}
	
	@Override
	public Object[] toArray() {
		// BEGIN (write your solution here)
		T[] arr = (T[]) new Object[size];
        Item<T> currEl = firstInList;
        
        if(firstInList == null) return arr;

        int i = 0;
        while(currEl.getNextItem() != null) {
            arr[i] = currEl.getValue();
            i++;
            currEl = currEl.getNextItem();
        }
        return arr;
		// END
	}
	
	@Override
	public <T1> T1[] toArray(T1[] a) {
		// BEGIN (write your solution here)
		if (a.length < size) {
            a = (T1[])java.lang.reflect.Array.newInstance(
                                a.getClass().getComponentType(), size);
        }

        int i = 0;
        Object[] result = a;
        for (Item<T> x = firstInList; x != null; x = x.getNextItem()) {
            result[i++] = x.getValue();
        }

        if (a.length > size)
            a[size] = null;

        return a;
		// END
	}
	
	@Override
	public boolean add(final T newElement) {
		// BEGIN (write your solution here)
		Item<T> newItem = new Item<>(newElement, lastInList, null);
        if (lastInList == null)
            firstInList = newItem;
        else
            lastInList.nextItem = newItem;
        lastInList = newItem;
        size++;
        return true;
		// END
	}
	
	@Override
	public boolean remove(final Object o) {
		// BEGIN (write your solution here)
        if (o == null) {
            for (Item<T> x = lastInList; x != null; x = x.prevItem) {
                if (x.element == null) {
                    unlink(x);
                    return true;
                }
            }
        } else {
            for (Item<T> x = lastInList; x != null; x = x.prevItem) {
                if (o.equals(x.element)) {
                    unlink(x);
                    return true;
                }
            }
        }
        return false;
		// END
	}

    T unlink(Item<T> x) {
        // assert x != null;
        final T element = x.element;
        final Item<T> next = x.nextItem;
        final Item<T> prev = x.prevItem;

        if (prev == null) {
            firstInList = next;
        } else {
            prev.nextItem = next;
            x.prevItem = null;
        }

        if (next == null) {
            lastInList = prev;
        } else {
            next.prevItem = prev;
            x.nextItem = null;
        }

        x.element = null;
        size--;

        return element;
    }
	
	@Override
	public boolean containsAll(final Collection<?> c) {
		for (final Object item : c) {
			if (!this.contains(item)) return false;
		}
		return true;
	}
	
	@Override
	public boolean addAll(final Collection<? extends T> c) {
		for (final T item : c) {
			add(item);
		}
		return true;
	}
	
	@Override
	public boolean removeAll(final Collection<?> c) {
		for (final Object item : c) {
			remove(item);
		}
		return true;
	}
	
	@Override
	public boolean retainAll(final Collection<?> c) {
		for (final T item : this) {
			if (!c.contains(item)) this.remove(item);
		}
		return true;
	}
	
	@Override
	public void clear() {
		// BEGIN (write your solution here)
		firstInList = null;
        lastInList = null;
        size = 0;
		// END
	}
	
	@Override
	public T remove(final int index) throws IndexOutOfBoundsException {
		// BEGIN (write your solution here)
        if(index < 0 || index >= size()) throw new IndexOutOfBoundsException();

        if (index == 0) {
            Item<T> item = firstInList;
            if (firstInList == lastInList) {
                this.clear();
                return item.element;
            }
            item.nextItem.prevItem = null;
            firstInList = item.nextItem;
            size--;
            return item.element;
        }

        if (index == size - 1) {
            Item<T> item = lastInList;
            item.prevItem.nextItem = null;
            lastInList = item.prevItem;
            size--;
            return item.element;
        }

        Item<T> item = firstInList;

        for (int i = 0; i < index; i++){
            item = item.nextItem;
        }

        item.nextItem.prevItem = item.prevItem;
        item.prevItem.nextItem = item.nextItem;
        size--;

        return item.element;
		// END
	}
	
	
	private void remove(final Item<T> current) {
		// BEGIN (write your solution here)
		remove(indexOf(current.element));
		// END
	}
	
	@Override
	public List<T> subList(final int start, final int end) {
		return null;
	}
	
	@Override
	public ListIterator<T> listIterator() {
		return new ElementsIterator(0);
	}
	
	@Override
	public ListIterator<T> listIterator(final int index) {
		return new ElementsIterator(index);
	}
	
	@Override
	public int lastIndexOf(final Object target) {
		throw new UnsupportedOperationException();
	}
	
	@Override
	public int indexOf(final Object o) {
		// BEGIN (write your solution here)
        int index = 0;

        if (o == null) {
            for (Item<T> x = firstInList; x != null; x = x.nextItem) {
                if (x.element == null)
                    return index;
                index++;
            }
        } else {
            for (Item<T> x = firstInList; x != null; x = x.nextItem) {
                if (o.equals(x.element))
                    return index;
                index++;
            }
        }
        return -1;
		// END
	}
	
	@Override
	public void add(final int index, final T element) {
		throw new UnsupportedOperationException();
	}
	
	@Override
	public boolean addAll(final int index, final Collection elements) {
		throw new UnsupportedOperationException();
	}
	
	@Override
	public T set(final int index, final T element) {
		// BEGIN (write your solution here)
        if(index < 0 || index >= size()) throw new IndexOutOfBoundsException();
        Item<T> item = firstInList;
        for (int i = 0; i < index; i++) {
            item = item.nextItem;
        }
        T value = item.element;
        item.element = element;
        
        return value;
		// END
	}
	
	@Override
	public T get(final int index) throws IndexOutOfBoundsException {
		// BEGIN (write your solution here)
        if(index < 0 || index >= size()) throw new IndexOutOfBoundsException();
		Item<T> currEl = firstInList;
        int i = 0;
        while(currEl.getNextItem() != null) {
            if(i == index) {
                break;
            }
            i++;
            currEl = currEl.getNextItem();
        }
        return currEl.getValue();
		// END
	}
	
	private Item<T> getItemByIndex(final int index) throws IndexOutOfBoundsException {
		// BEGIN (write your solution here) An auxiliary method for searching for node by index.
		if(index < 0 || index >= size()) throw new IndexOutOfBoundsException();
		Item<T> currEl = firstInList;
        int i = 0;
        while(currEl.getNextItem() != null) {
            if(i == index) {
                break;
            }
            i++;
            currEl = currEl.getNextItem();
        }
        return currEl;
		// END
	}
	
	private class ElementsIterator implements ListIterator<T> {

        private Item<T> currentItemInIterator;
        private Item<T> lastReturnedItemFromIterator;
        private int index;

        public ElementsIterator() {
            this(0);
        }

        public ElementsIterator(final int index) {
            // BEGIN (write your solution here)
            this.currentItemInIterator = (index == size) ? null : getItemByIndex(index);
            this.index = index;
            // END
        }

        @Override
        public boolean hasNext() {
            return this.index < size;
        }

        @Override
        public T next() {
            // BEGIN (write your solution here)
            if (!hasNext()) throw new NoSuchElementException();

            lastReturnedItemFromIterator = currentItemInIterator;
            currentItemInIterator = currentItemInIterator.getNextItem();
            index++;

            return lastReturnedItemFromIterator.element;
            // END
        }

        @Override
        public void add(final T element) {
            LinkedList.this.add(element);
        }

        @Override
        public void set(final T element) {
            // BEGIN (write your solution here)
            if (lastReturnedItemFromIterator == null) throw new IllegalStateException();
            lastReturnedItemFromIterator.element = element;
            // END
        }

        @Override
        public int previousIndex(){
            // BEGIN (write your solution here)
            return index - 1;
            // END
        }

        @Override
        public int nextIndex() {
            // BEGIN (write your solution here)
            return index;
            // END
        }

        @Override
        public boolean hasPrevious() {
            // BEGIN (write your solution here)
            return index > 0;
            // END
        }

        @Override
        public T previous() {
            // BEGIN (write your solution here)
            if (!hasPrevious()) throw new NoSuchElementException();

            if (currentItemInIterator == null) {
                lastReturnedItemFromIterator = currentItemInIterator = lastInList;
            } else {
                lastReturnedItemFromIterator = currentItemInIterator = currentItemInIterator.getPrevItem();
            }
            index--;

            return lastReturnedItemFromIterator.element;
            // END
        }

        @Override
        public void remove() {
            // BEGIN (write your solution here)
            if (lastReturnedItemFromIterator == null) throw new IllegalStateException();
            LinkedList.this.remove(lastReturnedItemFromIterator);
            lastReturnedItemFromIterator = null;
            index--;
            // END
        }
    }
	
	private static class Item<T> {
		
		private T element;
		
		private Item<T> nextItem;
		
		private Item<T> prevItem;
		
		Item(final T element, final Item<T> prevItem, final Item<T> nextItem) {
			this.element = element;
			this.nextItem = nextItem;
			this.prevItem = prevItem;
		}
		
		
		public Item<T> getNextItem() {
			return nextItem;
		}
		
		public Item<T> getPrevItem() {
			return prevItem;
		}

        public T getValue() {
            return this.element;
        }
	}
}

