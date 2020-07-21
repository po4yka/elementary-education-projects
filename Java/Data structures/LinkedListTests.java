import org.junit.Test;

import java.util.Collection;
import java.util.Iterator;
import java.util.List;
import java.util.ListIterator;
import static org.junit.Assert.*;

public class LinkedListTest {

    @Test
    public void testSizeWhenSizeIs0()  {
        final LinkedList<Integer> testInstance = new LinkedList<>();
        assertEquals("The list shows the wrong size!", 0, testInstance.size());
    }

    @Test
    public void testIsEmptyWhenEmpty()  {
        final LinkedList<Integer> testInstance = new LinkedList<>();
        assertTrue("The list is definitely not empty ?!", testInstance.isEmpty());
    }

    @Test
    public void testContains() {
        final LinkedList<Integer> testInstance = new LinkedList<>();
        testInstance.add(1);
        testInstance.add(2);

        assertTrue("The list cannot find an existing element in itself! Where is the number one gone?",
                testInstance.contains(1));
        assertTrue("The list cannot find an existing element in itself! Where did the number two go?",
                testInstance.contains(2));
        assertFalse("The list could find in itself a nonexistent element! We did not add zero to it, but it was found there))",
                testInstance.contains(0));
    }

    @Test
    public void testContainsOnNull() {
        final LinkedList<Object> testInstance = new LinkedList<>();
        testInstance.add(1);
        testInstance.add(null);
        testInstance.add("s");

        assertTrue("The list cannot find an existing element in itself! Where is the number one gone?",
                testInstance.contains(1));
        assertTrue("The list cannot find an existing element in itself! Where is the null gone?",
                testInstance.contains(null));
        assertTrue("The list cannot find an existing element in itself! Where did the letter s go?",
                testInstance.contains("s"));
        assertFalse("The list could find in itself a nonexistent element!",
                testInstance.contains("d"));
    }

    @Test
    public void testToArrayWhenInputArrayHaveSizeOne()  {
        final LinkedList<Integer> testInstance = new LinkedList<>();
        testInstance.add(1);
        testInstance.add(2);
        testInstance.add(3);

	final Integer[] input = new Integer[1];

	final Integer[] result = testInstance.toArray(input);
	assertNotEquals("Incoming and outgoing arrays cannot be the same!",
            input, result);
	assertEquals("The first element of the list was not transferred to the resulting array!",
            (Integer)1, result[0]);
	assertEquals("The second list item was not transferred to the resulting array!",
            (Integer)2, result[1]);
	assertEquals("The third element of the list was not transferred to the resulting array!",
            (Integer)3, result[2]);
	assertEquals("The length of the returned array is wrong!",
            3, result.length);
    }

    @Test
    public void testToArrayWhenInputArrayHaveCorrectSize() {
        final LinkedList<Integer> testInstance = new LinkedList<>();
        testInstance.add(1);
        testInstance.add(2);
        testInstance.add(3);

	final Integer[] input = new Integer[3];

	final Integer[] result = testInstance.toArray(input);
        assertArrayEquals("If the size of the incoming array is equal to the size of the list, then the list should return in the same array, and not in the new one!",
                input, result);
    }


    @Test
    public void testAdd()  {
        final LinkedList<Integer> testInstance = new LinkedList<>();
        testInstance.add(1);
        testInstance.add(1);

        assertEquals("After adding an item - the size of the list has not changed!",
                2, testInstance.size());
        assertFalse("After adding two elements - the list should not think that it is empty!",
                testInstance.isEmpty());
    }

    @Test
    public void testRemoveFirstIndex()  {
        final LinkedList<String> testInstance = new LinkedList<>();
        testInstance.add("a");
        testInstance.add("b");
        testInstance.add("c");
        testInstance.remove(0);

        assertEquals("After deleting one item - the size of the list has not changed!",
                2, testInstance.size());
        assertFalse("After deleting one of the three elements - the list should not consider itself empty!",
                testInstance.isEmpty());
    }

    @Test
    public void testRemoveLastIndex()  {
        final LinkedList<String> testInstance = new LinkedList<>();
        testInstance.add("a");
        testInstance.add("b");
        testInstance.add("c");
        testInstance.remove(2);
        testInstance.remove(1);

        assertEquals("After deleting two items - the size of the list has not changed!",
                1, testInstance.size());
        assertFalse("After deleting two of the three elements - the list should not consider itself empty!",
                testInstance.isEmpty());
    }

    @Test
    public void testRemoveFirstItem()  {
        final LinkedList<String> testInstance = new LinkedList<>();
        testInstance.add("a");
        testInstance.add("b");
        testInstance.add("c");
        testInstance.remove("a");

        assertEquals("After deleting one item - the size of the list has not changed!",2,
                testInstance.size());
        assertFalse("After deleting one of the three elements - the list should not consider itself empty!",
                testInstance.isEmpty());
    }

    @Test
    public void testRemoveLastItem()  {
        final LinkedList<String> testInstance = new LinkedList<>();
        testInstance.add("a");
        testInstance.add("b");
        testInstance.add("c");
        testInstance.remove("c");
        testInstance.remove("b");

        assertEquals("After deleting two items - the size of the list has not changed!",
                1, testInstance.size());
        assertFalse("After deleting two of the three elements - the list should not consider itself empty!",
                testInstance.isEmpty());
    }

    @Test
    public void testContainsAll() {
        final LinkedList<Integer> testInstance = new LinkedList<>();
        final LinkedList<Integer> testInstance2 = new LinkedList<>();
        testInstance.add(1);
        testInstance.add(2);

        testInstance2.add(2);
        testInstance2.add(1);

        assertTrue("The containsAll method does not work correctly!",
                testInstance.containsAll(testInstance2));
    }

    @Test
    public void testAddAll(){
        final LinkedList<Integer> testInstance = new LinkedList<>();
        final LinkedList<Integer> testInstance2 = new LinkedList<>();
        testInstance.add(1);
        testInstance.add(2);

        testInstance2.add(3);
        testInstance2.add(4);
        
        testInstance.addAll(testInstance2);

        assertTrue("The list does not contain items added by the addAll method!",
                testInstance.contains(3));
        assertTrue("The list does not contain items added by the addAll method!",
                testInstance.contains(4));
    }

    @Test
    public void testRemoveAll() {
        final LinkedList<Integer> testInstance = new LinkedList<>();
        final LinkedList<Integer> testInstance2 = new LinkedList<>();
        testInstance.add(1);
        testInstance.add(2);
        testInstance.add(3);

        testInstance2.add(2);
        testInstance2.add(3);
        testInstance2.add(4);

        testInstance.removeAll(testInstance2);

        assertEquals("removeAll method removed the wrong number of elements!",
                1, testInstance.size());
        assertTrue("After the removeAll method, the element disappeared!",
                testInstance.contains(1));
        assertFalse("The removeAll method did not remove the item from the list for deletion!",
                testInstance.contains(2));
        assertFalse("The removeAll method did not remove the item from the list for deletion!",
                testInstance.contains(3));
    }

    @Test
    public void testRetainAll()  {
        final LinkedList<Integer> testInstance = new LinkedList<>();
        final LinkedList<Integer> testInstance2 = new LinkedList<>();
        testInstance.add(1);
        testInstance.add(2);

        testInstance2.add(2);
        testInstance2.add(3);

        testInstance.retainAll(testInstance2);

        assertEquals("The retainAll method does not work well!",
                1, testInstance.size());
        assertTrue("After retainAll method, the item was lost!",
                testInstance.contains(2));
    }

    @Test
    public void testClear(){
        final LinkedList<Integer> testInstance = new LinkedList<>();
        testInstance.add(1);
        testInstance.add(1);

        testInstance.clear();

        assertTrue("After cleaning, the list thinks that it is NOT EMPTY!",
                testInstance.isEmpty());
        assertEquals("After cleaning, the list size is wrong!",
                0, testInstance.size());
    }

    @Test
    public void testRemoveByIndex() throws RuntimeException {
        final LinkedList<String> testInstance = new LinkedList<>();
        testInstance.add("a");
        testInstance.add("b");
        testInstance.add("c");
        testInstance.add("d");

        try {
            testInstance.remove(5);
        } catch (final NullPointerException e1) {
            throw new RuntimeException(
                    "The remove(index) method did not check the marginal values of index. " +
                            "IndexOutOfBoundsException was expected. " +
                            "So get the NullPointerException.", e1);
        } catch (final IndexOutOfBoundsException e) {}


        assertEquals("Method remove(index) must return deleted element", "b", testInstance.remove(1));
        assertEquals("After remove size is incorrect", 3, testInstance.size());

        assertEquals("Method remove(index) could not delete the last element", "d", testInstance.remove(2));
        assertEquals("After remove size is incorrect", 2, testInstance.size());

        assertEquals("Method remove(index) could not delete the first element", "a", testInstance.remove(0));
        assertEquals("After remove size is incorrect", 1, testInstance.size());

        testInstance.remove(0);
        assertEquals("After remove size is incorrect", 0, testInstance.size());

    }

    @Test
    public void testSetInList() {
        final LinkedList<Object> testInstance = new LinkedList<>();
        testInstance.add(1);
        testInstance.add(2);
        testInstance.add("trololo");

        assertEquals("The method must return the replaced element.", "trololo", testInstance.set(2, 1));
        assertEquals("The set() method touched other elements that shouldn't have been!",
                1, testInstance.get(0));
        assertEquals("The set() method touched other elements that shouldn't have been!",
                2, testInstance.get(1));
    }

    @Test
    public void testSetInListOnException() {
        final LinkedList<Object> testInstance = new LinkedList<>();
        testInstance.add(1);
        testInstance.add(2);
        testInstance.add("trololo");


        try {
            testInstance.set(3, -25);
            fail("The set () method should throw an exception on the invalid inbound index!");
        } catch (Exception e) {}

        try {
            testInstance.set(-1, 25);
            fail("The set () method should throw an exception on the invalid inbound index!");
        } catch (Exception e) {}
    }

    @Test
    public void testGetInList() {
        final LinkedList<Object> testInstance = new LinkedList<>();
        testInstance.add(1);
        testInstance.add(2);
        testInstance.add("trololo");

        assertEquals("The get() method returned the wrong item!",
                "trololo", testInstance.get(2));
        assertEquals("The get() method returned the wrong item!",
                1, testInstance.get(0));
        assertEquals("The get() method returned the wrong item!",
                2, testInstance.get(1));
    }

    @Test
    public void testGetInListOnException() {
        final List<Object> testInstance = new LinkedList<>();
        testInstance.add(1);
        testInstance.add(2);
        testInstance.add("trololo");


        try {
            int temp = (int)testInstance.get(-25);
            fail("The get() method should throw an exception on the invalid inbound index!" +
		            "Expected: IndexOutOfBoundsException, Current: " + temp);
        } catch (Exception e) {}

        try {
	        int temp = (int)testInstance.get(3);
	        fail("The get() method should throw an exception on the invalid inbound index!" +
			        "Expected: IndexOutOfBoundsException, Current: " + temp);
        } catch (Exception e) {}
    }

    @Test
    public void testIndexOf() {
        final LinkedList<Object> testInstance = new LinkedList<>();
        testInstance.add(1);
        testInstance.add(2);
        testInstance.add("trololo");
        testInstance.add(null);
        ///testInstance.indexOf(2);
        assertEquals("The indexOf() method did not find the correct index for element 2!",
                1, testInstance.indexOf(2));
        assertEquals("The indexOf() method did not find the correct index for the \"trololo\" element!",
                2, testInstance.indexOf("trololo"));
        assertEquals("The indexOf() method did not find the correct index for the `null` element!",
                3, testInstance.indexOf(null));
    }

    //ITERATOR TESTS


    @Test
    public void testHasNextInEmptyList() {
        final LinkedList<Integer> testInstance = new LinkedList<>();
        final ListIterator<Integer> iter = testInstance.listIterator();
        assertFalse(iter.hasNext());
    }
    @Test
    public void testHasNext() {
        final LinkedList<Integer> testInstance = new LinkedList<>();
        testInstance.add(3);
        final ListIterator<Integer> iter = testInstance.listIterator();
        assertTrue(iter.hasNext());
        testInstance.add(3);
        testInstance.add(3);
        assertTrue(iter.hasNext());
    }


    @Test
    public void testNextOnEmptyCollection()  {
        final LinkedList<Integer> testInstance = new LinkedList<>();
        final LinkedList<Integer> testInstance2 = new LinkedList<>();
        testInstance.add(1);
        testInstance.add(2);

        final ListIterator<Integer> iter = testInstance.listIterator();
        iter.next();
        iter.remove();
        iter.next();
        iter.remove();
        try {
            iter.next();
            fail("next does not throw the Exception when no more elements");
        } catch (final java.util.NoSuchElementException e) {}
        final ListIterator<Integer> iter2 = testInstance2.listIterator();
        try {
            iter2.next();
            fail("next does not throw the Exception when no  elements");
        } catch (final java.util.NoSuchElementException e) {}
    }

    @Test
    public void testNextOnOneElementInCollection()  {
        final LinkedList<String> testInstance = new LinkedList<>();
        testInstance.add("a");


        final Iterator<String> iter = testInstance.iterator();
        iter.next();

    }
    @Test
    public void testNextAfterPrevious()  {
        final LinkedList<String> testInstance = new LinkedList<>();
        testInstance.add("a");
        testInstance.add("b");
        testInstance.add("c");
    
        final ListIterator<String> iter = testInstance.listIterator(3);
        iter.previous();
        iter.next();
        iter.previous();
        iter.previous();
        iter.next();
        iter.previous();
        iter.previous();
        iter.next();
    }

    @Test
    public void testHasPreviousWhenIteratorAtTheEndOfTheCollection() {
        final LinkedList<Integer> testInstance = new LinkedList<>();
        testInstance.add(1);
        testInstance.add(2);

        final ListIterator<Integer> listIterator = testInstance.listIterator();
        listIterator.next();

        assertTrue(listIterator.hasPrevious());
    }

    @Test
    public void testHasPreviousWhenIteratorAtTheBeginningOfTheCollection() {
        final LinkedList<Integer> testInstance = new LinkedList<>();
        testInstance.add(1);
        testInstance.add(2);

        final ListIterator<Integer> listIterator = testInstance.listIterator();
        listIterator.next();
        listIterator.previous();
        assertFalse("The hasPrevious() method must return false when the" +
                " iterator is on the first LinkedList element.",listIterator.hasPrevious());
    }

    @Test
    public void testHasPreviousWhenEmptyCollection() {
        final LinkedList<Integer> testInstance = new LinkedList<>();

        final ListIterator<Integer> listIterator = testInstance.listIterator();

        assertFalse(listIterator.hasPrevious());
    }

    @Test
    public void testPreviousOnCollectionWithOneElement() {
        final LinkedList<Integer> testInstance = new LinkedList<>();
        testInstance.add(1);

        final ListIterator<Integer> listIterator = testInstance.listIterator();
        final Integer next = listIterator.next();
        final Integer previous = listIterator.previous();

        assertEquals(next, previous);
    }

    @Test
    public void testPreviousWhenEmptyCollection() {
        final LinkedList<Integer> testInstance = new LinkedList<>();

        final ListIterator<Integer> listIterator = testInstance.listIterator();

        try {
            listIterator.previous();
            fail("list iterator do not throw the Exception when called previous method on empty collection");
        } catch (final java.util.NoSuchElementException e) {}
    }

    @Test
    public void testPreviousAfterNext() {
        final LinkedList<Integer> testInstance = new LinkedList<>();
        testInstance.add(0);
        testInstance.add(1);
        testInstance.add(2);
        testInstance.add(3);
        testInstance.add(4);
        testInstance.add(5);

        final ListIterator<Integer> listIterator = testInstance.listIterator();
        while (listIterator.hasNext()) listIterator.next();
        while (listIterator.hasPrevious()) listIterator.previous();
        assertEquals(listIterator.next(), listIterator.previous());
    }

    @Test
    public void testPreviousSeveralTimesOneByOneFromTheEndOfList() {
        final LinkedList<Integer> testInstance = new LinkedList<>();
        testInstance.add(0);
        testInstance.add(1);
        testInstance.add(2);
        testInstance.add(3);
        testInstance.add(4);
        testInstance.add(5);

        final ListIterator<Integer> listIterator = testInstance.listIterator(6);
        listIterator.previous();
        listIterator.previous();
        listIterator.previous();
        listIterator.previous();
        listIterator.previous();
        listIterator.previous();
    }

    @Test
    public void testIteratorSet() {
        final LinkedList<Integer> testInstance = new LinkedList<>();
        testInstance.add(1);
        testInstance.add(2);
        testInstance.add(3);
        testInstance.add(4);

        final ListIterator<Integer> listIterator = testInstance.listIterator(2);
        listIterator.next();
        listIterator.set(0);
        assertEquals((Integer)0, testInstance.get(2));
        listIterator.previous();
        listIterator.previous();
        listIterator.set(9);
        assertEquals((Integer)9, testInstance.get(1));
    }

    @Test // Iterator
    public void testIteratorSetWhenNeitherNextNorPreviousHaveBeenCalled() {
        final LinkedList<Integer> testInstance = new LinkedList<>();
        testInstance.add(1);

        final ListIterator<Integer> listIterator = testInstance.listIterator();

        try {
            listIterator.set(null);
            fail("set method do not throw IllegalStateException the if neither next nor previous have been called");
        } catch (final IllegalStateException e){}
    }

    @Test // Iterator
    public void testPreviousIndex() {
        final LinkedList<Integer> testInstance = new LinkedList<>();
        testInstance.add(1);

        final ListIterator<Integer> listIterator = testInstance.listIterator();
        listIterator.next();

        assertEquals(0, listIterator.previousIndex());
    }

    @Test // Iterator
    public void testPreviousIndexWhenItEqualsTo1() {
        final LinkedList<Integer> testInstance = new LinkedList<>();
        testInstance.add(1);
        testInstance.add(1);

        final ListIterator<Integer> listIterator = testInstance.listIterator();
        listIterator.next();
        listIterator.next();

        assertEquals(1, listIterator.previousIndex());
    }

    @Test // Iterator
    public void testPreviousIndexWhenEmptyCollection() {
        final LinkedList<Integer> testInstance = new LinkedList<>();

        final ListIterator<Integer> listIterator = testInstance.listIterator();

        assertEquals(-1, listIterator.previousIndex());
    }

    @Test // Iterator
    public void testNextIndex() {
        final LinkedList<String> testInstance = new LinkedList<String>(){
            {
                add("a");
                add("b");
                add("c");
                add("d");
                add("e");
            }
        };
        final ListIterator<String> listIterator = testInstance.listIterator(5);
        assertEquals(5, listIterator.nextIndex());
        assertEquals("e", listIterator.previous());
        assertEquals(4, listIterator.nextIndex());
        assertEquals("e", listIterator.next());
        assertEquals(5, listIterator.nextIndex());
    }

    @Test // Iterator
    public void testNextIndexInEmptyList() {
        final LinkedList testInstance = new LinkedList<>();
        final ListIterator listIterator = testInstance.listIterator();
        assertEquals(0, listIterator.nextIndex());
        assertEquals(testInstance.size(), listIterator.nextIndex());
    }

    @Test // Iterator
    public void testRemoveTwoTimeInTheRow() {
        final Collection<Integer> testInstance = new LinkedList<>();
        testInstance.add(1);
        testInstance.add(2);

        final Iterator<Integer> iter = testInstance.iterator();
        iter.next();
        iter.remove();
        assertEquals("Expected collection size is 1, however actual is not", 1, testInstance.size());
        try {
            iter.remove();
            fail("remove do not throw the Exception when called twice");
        } catch (final IllegalStateException e) {}
    }


    @Test // Iterator
    public void testRemoveAfterPrevious()  {
        final LinkedList<Integer> testInstance = new LinkedList<>();
        testInstance.add(1);
        testInstance.add(2);

        final ListIterator<Integer> listIterator = testInstance.listIterator();
        listIterator.next();
        listIterator.next();
        listIterator.previous();
        listIterator.remove();
    }

    @Test // Iterator
    public void testRemoveBeforeNext() {
        final LinkedList<Integer> testInstance = new LinkedList<>();
        testInstance.add(2);

        final Iterator<Integer> iter = testInstance.iterator();
        try {
            iter.remove();
            fail("remove do not throw the Exception when called before next");
        } catch (final IllegalStateException e) {}
    }
    
    @Test // Iterator
    public void testRemoveInIterator() {
        final LinkedList<String> testInstance = new LinkedList<>();
        testInstance.add("a");
        testInstance.add("b");
        testInstance.add("c");
        testInstance.add("d");
        
        Iterator<String> iter = testInstance.listIterator();
        
        String tempElement = iter.next();
        iter.remove();
        assertFalse("The remove() method, in the iterator, did not delete" +
                " the element that was returned by the next() method.",
                testInstance.contains(tempElement));
        assertEquals("The size of the collection after the remove()" +
                " method in the iterator is incorrect.", 3, testInstance.size());
    }
}

