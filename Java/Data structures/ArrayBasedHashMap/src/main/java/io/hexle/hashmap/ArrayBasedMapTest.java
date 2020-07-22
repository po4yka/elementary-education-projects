import org.junit.Test;

import java.util.*;
import java.util.Map.Entry;

import static org.junit.Assert.*;

/*
 * Copyright (C) 2008
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
public class ArrayBasedMapTest {

    protected final boolean supportsPut;
    protected final boolean supportsRemove;
    protected final boolean supportsClear;
    protected final boolean allowsNullKeys;
    protected final boolean allowsNullValues;
    protected final boolean supportsIteratorRemove;

    /**
     * Creates a new, empty instance of the class under test.
     *
     * @return a new, empty map instance.
     * @throws UnsupportedOperationException if it's not possible to make an
     * empty instance of the class under test.
     */
    protected Map<String, Student> makeEmptyMap() {
        return new ArrayBasedMap<>();
    }

    /**
     * Creates a new, non-empty instance of the class under test.
     *
     * @return a new, non-empty map instance.
     * @throws UnsupportedOperationException if it's not possible to make a
     * non-empty instance of the class under test.
     */
    protected Map<String, Student> makePopulatedMap() {
        return new ArrayBasedMap<String, Student>(){{
            for (int i = 0; i < 1_000; i++) {
                put(String.valueOf(i), new Student(String.valueOf(i)));
            }
        }};
    }

    /**
     * Creates a new key that is not expected to be found
     * in {@link #makePopulatedMap()}.
     *
     * @return a key.
     * @throws UnsupportedOperationException if it's not possible to make a key
     * that will not be found in the map.
     */
    protected String getKeyNotInPopulatedMap() {
        return "keyNotInMap";
    }

    /**
     * Creates a new value that is not expected to be found
     * in {@link #makePopulatedMap()}.
     *
     * @return a value.
     * @throws UnsupportedOperationException if it's not possible to make a value
     * that will not be found in the map.
     */
    protected  Student getValueNotInPopulatedMap() {
        return new Student("notInMap");
    }

    /**
     * Constructor that assigns {@code supportsIteratorRemove} the same value as
     * {@code supportsRemove}.
     */
    public ArrayBasedMapTest() {
        this(false, false, true, true, true, true);
    }

    /**
     * Constructor with an explicit {@code supportsIteratorRemove} parameter.
     */
    protected ArrayBasedMapTest(
            boolean allowsNullKeys,
            boolean allowsNullValues,
            boolean supportsPut,
            boolean supportsRemove,
            boolean supportsClear,
            boolean supportsIteratorRemove) {
        this.supportsPut = supportsPut;
        this.supportsRemove = supportsRemove;
        this.supportsClear = supportsClear;
        this.allowsNullKeys = allowsNullKeys;
        this.allowsNullValues = allowsNullValues;
        this.supportsIteratorRemove = supportsIteratorRemove;
    }

    /**
     * Used by tests that require a map, but don't care whether it's
     * populated or not.
     *
     * @return a new map instance.
     */
    protected Map<String, Student> makeEitherMap() {
        try {
            return makePopulatedMap();
        } catch (UnsupportedOperationException e) {
            return makeEmptyMap();
        }
    }

    protected final boolean supportsValuesHashCode(Map<String, Student> map) {
        // get the first non-null value
        Collection<Student> values = map.values();
        for (Student value : values) {
            if (value != null) {
                try {
                    value.hashCode();
                } catch (Exception e) {
                    return false;
                }
                return true;
            }
        }
        return true;
    }

    /**
     * Checks all the properties that should always hold of a map. Also calls
     * {@link #assertMoreInvariants} to check invariants that are peculiar to
     * specific implementations.
     *
     * @see #assertMoreInvariants
     * @param map the map to check.
     */
    protected final void assertInvariants(Map<String, Student> map) {
        Set<String> keySet = map.keySet();
        Collection<Student> valueCollection = map.values();
        Set<Map.Entry<String, Student>> entrySet = map.entrySet();

        assertEquals(map.size() == 0, map.isEmpty());
        assertEquals(map.size(), keySet.size());
        assertEquals(keySet.size() == 0, keySet.isEmpty());
        assertEquals(!keySet.isEmpty(), keySet.iterator().hasNext());

        int expectedKeySetHash = 0;
        for (String key : keySet) {
            Student value = map.get(key);
            expectedKeySetHash += key != null ? key.hashCode() : 0;
            assertTrue(map.containsKey(key));
            assertTrue(map.containsValue(value));
            assertTrue(valueCollection.contains(value));
            assertTrue(valueCollection.containsAll(Collections.singleton(value)));
            assertTrue(allowsNullKeys || (key != null));
        }
        assertEquals(expectedKeySetHash, keySet.hashCode());

        assertEquals(map.size(), valueCollection.size());
        assertEquals(valueCollection.size() == 0, valueCollection.isEmpty());
        assertEquals(
                !valueCollection.isEmpty(), valueCollection.iterator().hasNext());
        for (Student value : valueCollection) {
            assertTrue(map.containsValue(value));
            assertTrue(allowsNullValues || (value != null));
        }

        assertEquals(map.size(), entrySet.size());
        assertEquals(entrySet.size() == 0, entrySet.isEmpty());
        assertEquals(!entrySet.isEmpty(), entrySet.iterator().hasNext());
        assertFalse(entrySet.contains("foo"));

        boolean supportsValuesHashCode = supportsValuesHashCode(map);
        if (supportsValuesHashCode) {
            int expectedEntrySetHash = 0;
            for (Entry<String, Student> entry : entrySet) {
                assertTrue(map.containsKey(entry.getKey()));
                assertTrue(map.containsValue(entry.getValue()));
                int expectedHash =
                        (entry.getKey() == null ? 0 : entry.getKey().hashCode()) ^
                                (entry.getValue() == null ? 0 : entry.getValue().hashCode());
                assertEquals(expectedHash, entry.hashCode());
                expectedEntrySetHash += expectedHash;
            }
            assertEquals(expectedEntrySetHash, entrySet.hashCode());
        }

        Object[] entrySetToArray1 = entrySet.toArray();
        assertEquals(map.size(), entrySetToArray1.length);
        assertTrue(Arrays.asList(entrySetToArray1).containsAll(entrySet));

        Entry<?, ?>[] entrySetToArray2 = new Entry<?, ?>[map.size() + 2];
        entrySetToArray2[map.size()] = mapEntry("foo", 1);
        assertSame(entrySetToArray2, entrySet.toArray(entrySetToArray2));
        assertNull(entrySetToArray2[map.size()]);
        assertTrue(Arrays.asList(entrySetToArray2).containsAll(entrySet));

        Object[] valuesToArray1 = valueCollection.toArray();
        assertEquals(map.size(), valuesToArray1.length);
        assertTrue(Arrays.asList(valuesToArray1).containsAll(valueCollection));

        Object[] valuesToArray2 = new Object[map.size() + 2];
        valuesToArray2[map.size()] = "foo";
        assertSame(valuesToArray2, valueCollection.toArray(valuesToArray2));
        assertNull(valuesToArray2[map.size()]);
        assertTrue(Arrays.asList(valuesToArray2).containsAll(valueCollection));

        assertMoreInvariants(map);
    }

    /**
     * Override this to check invariants which should hold true for a particular
     * implementation, but which are not generally applicable to every instance
     * of Map.
     *
     * @param map the map whose additional invariants to check.
     */
    protected void assertMoreInvariants(Map<String, Student> map) {
    }

    @Test
    public void testClear() {
        final Map<String, Student> map;
        try {
            map = makePopulatedMap();
        } catch (UnsupportedOperationException e) {
            return;
        }

        if (supportsClear) {
            map.clear();
            assertTrue(map.isEmpty());
        } else {
            try {
                map.clear();
                fail("Expected UnsupportedOperationException.");
            } catch (UnsupportedOperationException e) {
                // Expected.
            }
        }
        assertInvariants(map);
    }

    @Test
    public void testContainsKey() {
        final Map<String, Student> map;
        final String unmappedKey;
        try {
            map = makePopulatedMap();
            unmappedKey = getKeyNotInPopulatedMap();
        } catch (UnsupportedOperationException e) {
            return;
        }
        assertFalse(map.containsKey(unmappedKey));
        assertTrue(map.containsKey(map.keySet().iterator().next()));
        if (allowsNullKeys) {
            map.containsKey(null);
        } else {
            try {
                map.containsKey(null);
            } catch (NullPointerException optional) {
            }
        }
        assertInvariants(map);
    }

    @Test
    public void testContainsValue() {
        final Map<String, Student> map;
        final Student unmappedValue;
        try {
            map = makePopulatedMap();
            unmappedValue = getValueNotInPopulatedMap();
        } catch (UnsupportedOperationException e) {
            return;
        }
        assertFalse(map.containsValue(unmappedValue));
        assertTrue(map.containsValue(map.values().iterator().next()));
        if (allowsNullValues) {
            map.containsValue(null);
        } else {
            try {
                map.containsKey(null);
            } catch (NullPointerException optional) {
            }
        }
        assertInvariants(map);
    }

    @Test  public void testEntrySet() {
        final Map<String, Student> map;
        final Set<Entry<String, Student>> entrySet;
        try {
            map = makePopulatedMap();
        } catch (UnsupportedOperationException e) {
            return;
        }
        assertInvariants(map);

        entrySet = map.entrySet();
        final String unmappedKey;
        final Student unmappedValue;
        try {
            unmappedKey = getKeyNotInPopulatedMap();
            unmappedValue = getValueNotInPopulatedMap();
        } catch (UnsupportedOperationException e) {
            return;
        }
        for (Entry<String, Student> entry : entrySet) {
            assertNotEquals(unmappedKey, entry.getKey());
            assertNotEquals(unmappedValue, entry.getValue());
        }
    }

    @Test
    public void testEntrySetForEmptyMap() {
        final Map<String, Student> map;
        try {
            map = makeEmptyMap();
        } catch (UnsupportedOperationException e) {
            return;
        }
        assertInvariants(map);
    }

    @Test
    public void testEntrySetContainsEntryNullKeyPresent() {
        if (!allowsNullKeys || !supportsPut) {
            return;
        }
        final Map<String, Student> map;
        final Set<Entry<String, Student>> entrySet;
        try {
            map = makeEitherMap();
        } catch (UnsupportedOperationException e) {
            return;
        }
        assertInvariants(map);

        entrySet = map.entrySet();
        final Student unmappedValue;
        try {
            unmappedValue = getValueNotInPopulatedMap();
        } catch (UnsupportedOperationException e) {
            return;
        }

        map.put(null, unmappedValue);
        Entry<String, Student> entry = mapEntry(null, unmappedValue);
        assertTrue(entrySet.contains(entry));
        assertFalse(entrySet.contains(mapEntry(null, null)));
    }

    @Test
    public void testEntrySetContainsEntryNullKeyMissing() {
        final Map<String, Student> map;
        final Set<Entry<String, Student>> entrySet;
        try {
            map = makeEitherMap();
        } catch (UnsupportedOperationException e) {
            return;
        }
        assertInvariants(map);

        entrySet = map.entrySet();
        final Student unmappedValue;
        try {
            unmappedValue = getValueNotInPopulatedMap();
        } catch (UnsupportedOperationException e) {
            return;
        }
        Entry<String, Student> entry = mapEntry(null, unmappedValue);
        assertFalse(entrySet.contains(entry));
        assertFalse(entrySet.contains(mapEntry(null, null)));
    }

    @Test
    public void testEntrySetRemoveMissingKey() {
        final Map<String, Student> map;
        final String key;
        try {
            map = makeEitherMap();
            key = getKeyNotInPopulatedMap();
        } catch (UnsupportedOperationException e) {
            return;
        }

        Set<Entry<String, Student>> entrySet = map.entrySet();
        Entry<String, Student> entry
                = mapEntry(key, getValueNotInPopulatedMap());
        int initialSize = map.size();
        if (supportsRemove) {
            boolean didRemove = entrySet.remove(entry);
            assertFalse(didRemove);
        } else {
            try {
                boolean didRemove = entrySet.remove(entry);
                assertFalse(didRemove);
            } catch (UnsupportedOperationException optional) {}
        }
        assertEquals(initialSize, map.size());
        assertFalse(map.containsKey(key));
        assertInvariants(map);
    }

    @Test
    public void testEntrySetRemoveDifferentValue() {
        final Map<String, Student> map;
        try {
            map = makePopulatedMap();
        } catch (UnsupportedOperationException e) {
            return;
        }

        Set<Entry<String, Student>> entrySet = map.entrySet();
        String key = map.keySet().iterator().next();
        Entry<String, Student> entry
                = mapEntry(key, getValueNotInPopulatedMap());
        int initialSize = map.size();
        if (supportsRemove) {
            boolean didRemove = entrySet.remove(entry);
            assertFalse(didRemove);
        } else {
            try {
                boolean didRemove = entrySet.remove(entry);
                assertFalse(didRemove);
            } catch (UnsupportedOperationException optional) {}
        }
        assertEquals(initialSize, map.size());
        assertTrue(map.containsKey(key));
        assertInvariants(map);
    }

    @Test
    public void testEntrySetRemoveNullKeyPresent() {
        if (!allowsNullKeys || !supportsPut || !supportsRemove) {
            return;
        }
        final Map<String, Student> map;
        final Set<Entry<String, Student>> entrySet;
        try {
            map = makeEitherMap();
        } catch (UnsupportedOperationException e) {
            return;
        }
        assertInvariants(map);

        entrySet = map.entrySet();
        final Student unmappedValue;
        try {
            unmappedValue = getValueNotInPopulatedMap();
        } catch (UnsupportedOperationException e) {
            return;
        }

        map.put(null, unmappedValue);
        assertEquals(unmappedValue, map.get(null));
        assertTrue(map.containsKey(null));
        Entry<String, Student> entry = mapEntry(null, unmappedValue);
        assertTrue(entrySet.remove(entry));
        assertNull(map.get(null));
        assertFalse(map.containsKey(null));
    }

    @Test
    public void testEntrySetRemoveNullKeyMissing() {
        final Map<String, Student> map;
        try {
            map = makeEitherMap();
        } catch (UnsupportedOperationException e) {
            return;
        }

        Set<Entry<String, Student>> entrySet = map.entrySet();
        Entry<String, Student> entry
                = mapEntry(null, getValueNotInPopulatedMap());
        int initialSize = map.size();
        if (supportsRemove) {
            boolean didRemove = entrySet.remove(entry);
            assertFalse(didRemove);
        } else {
            try {
                boolean didRemove = entrySet.remove(entry);
                assertFalse(didRemove);
            } catch (UnsupportedOperationException optional) {}
        }
        assertEquals(initialSize, map.size());
        assertInvariants(map);
    }

    @Test
    public void testEntrySetRemoveAllNullFromEmpty() {
        final Map<String, Student> map;
        try {
            map = makeEmptyMap();
        } catch (UnsupportedOperationException e) {
            return;
        }

        Set<Entry<String, Student>> entrySet = map.entrySet();
        if (supportsRemove) {
            try {
                entrySet.removeAll(null);
                fail("Expected NullPointerException.");
            } catch (NullPointerException e) {
                // Expected.
            }
        } else {
            try {
                entrySet.removeAll(null);
                fail("Expected UnsupportedOperationException or NullPointerException.");
            } catch (UnsupportedOperationException | NullPointerException e) {
                // Expected.
            }
        }
        assertInvariants(map);
    }

    @Test
    public void testEntrySetRetainAllNullFromEmpty() {
        final Map<String, Student> map;
        try {
            map = makeEmptyMap();
        } catch (UnsupportedOperationException e) {
            return;
        }

        Set<Entry<String, Student>> entrySet = map.entrySet();
        if (supportsRemove) {
            try {
                entrySet.retainAll(null);
                // Returning successfully is not ideal, but tolerated.
                fail("Expected NullPointerException.");
            } catch (NullPointerException e) {
                // Expected.
            }
        } else {
            try {
                entrySet.retainAll(null);
                // We have to tolerate a successful return (Sun bug 4802647)
                fail("Expected UnsupportedOperationException or NullPointerException.");
            } catch (UnsupportedOperationException | NullPointerException e) {
                // Expected.
            }
        }
        assertInvariants(map);
    }

    @Test
    public void testEntrySetClear() {
        final Map<String, Student> map;
        try {
            map = makePopulatedMap();
        } catch (UnsupportedOperationException e) {
            return;
        }

        Set<Entry<String, Student>> entrySet = map.entrySet();
        if (supportsClear) {
            entrySet.clear();
            assertTrue(entrySet.isEmpty());
        } else {
            try {
                entrySet.clear();
                fail("Expected UnsupportedOperationException.");
            } catch (UnsupportedOperationException e) {
                // Expected.
            }
        }
        assertInvariants(map);
    }

    @Test
    public void testEntrySetSetValue() {
        // TODO: Investigate the extent to which, in practice, maps that support
        // put() also support Entry.setValue().
        if (!supportsPut) {
            return;
        }

        final Map<String, Student> map;
        final Student valueToSet;
        try {
            map = makePopulatedMap();
            valueToSet = getValueNotInPopulatedMap();
        } catch (UnsupportedOperationException e) {
            return;
        }

        Set<Entry<String, Student>> entrySet = map.entrySet();
        Entry<String, Student> entry = entrySet.iterator().next();
        final Student oldValue = entry.getValue();
        final Student returnedValue = entry.setValue(valueToSet);
        assertEquals(oldValue, returnedValue);
        assertEquals(valueToSet, map.get(entry.getKey()));
        assertInvariants(map);
    }

    @Test
    public void testEntrySetSetValueSameValue() {
        // TODO: Investigate the extent to which, in practice, maps that support
        // put() also support Entry.setValue().
        if (!supportsPut) {
            return;
        }

        final Map<String, Student> map;
        try {
            map = makePopulatedMap();
        } catch (UnsupportedOperationException e) {
            return;
        }

        Set<Entry<String, Student>> entrySet = map.entrySet();
        Entry<String, Student> entry = entrySet.iterator().next();
        final Student oldValue = entry.getValue();
        final Student returnedValue = entry.setValue(oldValue);
        assertEquals(oldValue, returnedValue);
        assertTrue(entrySet.contains(
                mapEntry(entry.getKey(), oldValue)));
        assertEquals(oldValue, map.get(entry.getKey()));
        assertInvariants(map);
    }

    @Test
    public void testEqualsForLargerMap() {
        if (!supportsPut) {
            return;
        }

        final Map<String, Student> map;
        final Map<String, Student> largerMap;
        try {
            map = makePopulatedMap();
            largerMap = makePopulatedMap();
            largerMap.put(getKeyNotInPopulatedMap(), getValueNotInPopulatedMap());
        } catch (UnsupportedOperationException e) {
            return;
        }

        assertNotEquals(map, largerMap);
    }

    @Test
    public void testEqualsForSmallerMap() {
        if (!supportsRemove) {
            return;
        }

        final Map<String, Student> map;
        final Map<String, Student> smallerMap;
        try {
            map = makePopulatedMap();
            smallerMap = makePopulatedMap();
            smallerMap.remove(smallerMap.keySet().iterator().next());
        } catch (UnsupportedOperationException e) {
            return;
        }

        assertNotEquals(map, smallerMap);
    }

    @Test
    public void testGet() {
        final Map<String, Student> map;
        try {
            map = makePopulatedMap();
        } catch (UnsupportedOperationException e) {
            return;
        }

        for (Entry<String, Student> entry : map.entrySet()) {
            assertEquals(entry.getValue(), map.get(entry.getKey()));
        }

        String unmappedKey;
        try {
            unmappedKey = getKeyNotInPopulatedMap();
        } catch (UnsupportedOperationException e) {
            return;
        }
        assertNull(map.get(unmappedKey));
    }

    @Test
    public void testGetForEmptyMap() {
        final Map<String, Student> map;
        String unmappedKey = null;
        try {
            map = makeEmptyMap();
            unmappedKey = getKeyNotInPopulatedMap();
        } catch (UnsupportedOperationException e) {
            return;
        }
        assertNull(map.get(unmappedKey));
    }

    @Test
    public void testGetNull() {
        Map<String, Student> map = makeEitherMap();
        if (allowsNullKeys) {
            if (allowsNullValues) {
                // TODO: decide what to test here.
            } else {
                assertEquals(map.containsKey(null), map.get(null) != null);
            }
        } else {
            try {
                map.get(null);
            } catch (NullPointerException optional) {
            }
        }
        assertInvariants(map);
    }

    @Test
    public void testHashCode() {
        final Map<String, Student> map;
        try {
            map = makePopulatedMap();
        } catch (UnsupportedOperationException e) {
            return;
        }
        assertInvariants(map);
    }

    @Test
    public void testHashCodeForEmptyMap() {
        final Map<String, Student> map;
        try {
            map = makeEmptyMap();
        } catch (UnsupportedOperationException e) {
            return;
        }
        assertInvariants(map);
    }

    @Test
    public void testPutNewKey() {
        final Map<String, Student> map = makeEitherMap();
        final String keyToPut;
        final Student valueToPut;
        try {
            keyToPut = getKeyNotInPopulatedMap();
            valueToPut = getValueNotInPopulatedMap();
        } catch (UnsupportedOperationException e) {
            return;
        }
        if (supportsPut) {
            int initialSize = map.size();
            Student oldValue = map.put(keyToPut, valueToPut);
            assertEquals(valueToPut, map.get(keyToPut));
            assertTrue(map.containsKey(keyToPut));
            assertTrue(map.containsValue(valueToPut));
            assertEquals(initialSize + 1, map.size());
            assertNull(oldValue);
        } else {
            try {
                map.put(keyToPut, valueToPut);
                fail("Expected UnsupportedOperationException.");
            } catch (UnsupportedOperationException e) {
                // Expected.
            }
        }
        assertInvariants(map);
    }

    @Test
    public void testPutExistingKey() {
        final Map<String, Student> map;
        final String keyToPut;
        final Student valueToPut;
        try {
            map = makePopulatedMap();
            valueToPut = getValueNotInPopulatedMap();
        } catch (UnsupportedOperationException e) {
            return;
        }
        keyToPut = map.keySet().iterator().next();
        if (supportsPut) {
            int initialSize = map.size();
            map.put(keyToPut, valueToPut);
            assertEquals(valueToPut, map.get(keyToPut));
            assertTrue(map.containsKey(keyToPut));
            assertTrue(map.containsValue(valueToPut));
            assertEquals(initialSize, map.size());
        } else {
            try {
                map.put(keyToPut, valueToPut);
                fail("Expected UnsupportedOperationException.");
            } catch (UnsupportedOperationException e) {
                // Expected.
            }
        }
        assertInvariants(map);
    }

    @Test
    public void testPutAllNewKey() {
        final Map<String, Student> map = makeEitherMap();
        final String keyToPut;
        final Student valueToPut;
        try {
            keyToPut = getKeyNotInPopulatedMap();
            valueToPut = getValueNotInPopulatedMap();
        } catch (UnsupportedOperationException e) {
            return;
        }
        final Map<String, Student> mapToPut = Collections.singletonMap(keyToPut, valueToPut);
        if (supportsPut) {
            int initialSize = map.size();
            map.putAll(mapToPut);
            assertEquals(valueToPut, map.get(keyToPut));
            assertTrue(map.containsKey(keyToPut));
            assertTrue(map.containsValue(valueToPut));
            assertEquals(initialSize + 1, map.size());
        } else {
            try {
                map.putAll(mapToPut);
                fail("Expected UnsupportedOperationException.");
            } catch (UnsupportedOperationException e) {
                // Expected.
            }
        }
        assertInvariants(map);
    }

    @Test
    public void testPutAllExistingKey() {
        final Map<String, Student> map;
        final String keyToPut;
        final Student valueToPut;
        try {
            map = makePopulatedMap();
            valueToPut = getValueNotInPopulatedMap();
        } catch (UnsupportedOperationException e) {
            return;
        }
        keyToPut = map.keySet().iterator().next();
        final Map<String, Student> mapToPut = Collections.singletonMap(keyToPut, valueToPut);
        int initialSize = map.size();
        if (supportsPut) {
            map.putAll(mapToPut);
            assertEquals(valueToPut, map.get(keyToPut));
            assertTrue(map.containsKey(keyToPut));
            assertTrue(map.containsValue(valueToPut));
        } else {
            try {
                map.putAll(mapToPut);
                fail("Expected UnsupportedOperationException.");
            } catch (UnsupportedOperationException e) {
                // Expected.
            }
        }
        assertEquals(initialSize, map.size());
        assertInvariants(map);
    }

    @Test
    public void testRemove() {
        final Map<String, Student> map;
        final String keyToRemove;
        try {
            map = makePopulatedMap();
        } catch (UnsupportedOperationException e) {
            return;
        }
        keyToRemove = map.keySet().iterator().next();
        if (supportsRemove) {
            int initialSize = map.size();
            Student expectedValue = map.get(keyToRemove);
            Student oldValue = map.remove(keyToRemove);
            assertEquals(expectedValue, oldValue);
            assertFalse(map.containsKey(keyToRemove));
            assertEquals(initialSize - 1, map.size());
        } else {
            try {
                map.remove(keyToRemove);
                fail("Expected UnsupportedOperationException.");
            } catch (UnsupportedOperationException e) {
                // Expected.
            }
        }
        assertInvariants(map);
    }

    @Test
    public void testRemoveMissingKey() {
        final Map<String, Student> map;
        final String keyToRemove;
        try {
            map = makePopulatedMap();
            keyToRemove = getKeyNotInPopulatedMap();
        } catch (UnsupportedOperationException e) {
            return;
        }
        if (supportsRemove) {
            int initialSize = map.size();
            assertNull(map.remove(keyToRemove));
            assertEquals(initialSize, map.size());
        } else {
            try {
                map.remove(keyToRemove);
                fail("Expected UnsupportedOperationException.");
            } catch (UnsupportedOperationException e) {
                // Expected.
            }
        }
        assertInvariants(map);
    }

    @Test
    public void testSize() {
        assertInvariants(makeEitherMap());
    }

    @Test
    public void testKeySetClear() {
        final Map<String, Student> map;
        try {
            map = makeEitherMap();
        } catch (UnsupportedOperationException e) {
            return;
        }

        Set<String> keySet = map.keySet();
        if (supportsClear) {
            keySet.clear();
            assertTrue(keySet.isEmpty());
        } else {
            try {
                keySet.clear();
                fail("Expected UnsupportedOperationException.");
            } catch (UnsupportedOperationException e) {
                // Expected.
            }
        }
        assertInvariants(map);
    }

    @Test
    public void testKeySetRemoveAllNullFromEmpty() {
        final Map<String, Student> map;
        try {
            map = makeEmptyMap();
        } catch (UnsupportedOperationException e) {
            return;
        }

        Set<String> keySet = map.keySet();
        if (supportsRemove) {
            try {
                keySet.removeAll(null);
                fail("Expected NullPointerException.");
            } catch (NullPointerException e) {
                // Expected.
            }
        } else {
            try {
                keySet.removeAll(null);
                fail("Expected UnsupportedOperationException or NullPointerException.");
            } catch (UnsupportedOperationException | NullPointerException e) {
                // Expected.
            }
        }
        assertInvariants(map);
    }

    @Test
    public void testKeySetRetainAllNullFromEmpty() {
        final Map<String, Student> map;
        try {
            map = makeEmptyMap();
        } catch (UnsupportedOperationException e) {
            return;
        }

        Set<String> keySet = map.keySet();
        if (supportsRemove) {
            try {
                keySet.retainAll(null);
                // Returning successfully is not ideal, but tolerated.
                fail("Expected  NullPointerException.");
            } catch (NullPointerException e) {
                // Expected.
            }
        } else {
            try {
                keySet.retainAll(null);
                // We have to tolerate a successful return (Sun bug 4802647)
                fail("Expected UnsupportedOperationException or NullPointerException.");
            } catch (UnsupportedOperationException | NullPointerException e) {
                // Expected.
            }
        }
        assertInvariants(map);
    }

    @Test
    public void testValues() {
        final Map<String, Student> map;
        final Collection<Student> valueCollection;
        try {
            map = makePopulatedMap();
        } catch (UnsupportedOperationException e) {
            return;
        }
        assertInvariants(map);

        valueCollection = map.values();
        final Student unmappedValue;
        try {
            unmappedValue = getValueNotInPopulatedMap();
        } catch (UnsupportedOperationException e) {
            return;
        }
        for (Student value : valueCollection) {
            assertNotEquals(unmappedValue, value);
        }
    }

    @Test
    public void testValuesRemoveMissing() {
        final Map<String, Student> map;
        final Student valueToRemove;
        try {
            map = makeEitherMap();
            valueToRemove = getValueNotInPopulatedMap();
        } catch (UnsupportedOperationException e) {
            return;
        }

        Collection<Student> valueCollection = map.values();
        int initialSize = map.size();
        if (supportsRemove) {
            assertFalse(valueCollection.remove(valueToRemove));
        } else {
            try {
                assertFalse(valueCollection.remove(valueToRemove));
                fail("Expected UnsupportedOperationException.");
            } catch (UnsupportedOperationException e) {
                // Tolerated.
            }
        }
        assertEquals(initialSize, map.size());
        assertInvariants(map);
    }

    @Test
    public void testValuesRemoveAllNullFromEmpty() {
        final Map<String, Student> map;
        try {
            map = makeEmptyMap();
        } catch (UnsupportedOperationException e) {
            return;
        }

        Collection<Student> values = map.values();
        if (supportsRemove) {
            try {
                values.removeAll(null);
                // Returning successfully is not ideal, but tolerated.
                fail("Expected NullPointerException.");
            } catch (NullPointerException e) {
                // Expected.
            }
        } else {
            try {
                values.removeAll(null);
                // We have to tolerate a successful return (Sun bug 4802647)
                fail("Expected UnsupportedOperationException or NullPointerException.");
            } catch (UnsupportedOperationException | NullPointerException e) {
                // Expected.
            }
        }
        assertInvariants(map);
    }

    @Test
    public void testValuesRetainAllNullFromEmpty() {
        final Map<String, Student> map;
        try {
            map = makeEmptyMap();
        } catch (UnsupportedOperationException e) {
            return;
        }

        Collection<Student> values = map.values();
        if (supportsRemove) {
            try {
                values.retainAll(null);
                // Returning successfully is not ideal, but tolerated.
                fail("Expected NullPointerException.");
            } catch (NullPointerException e) {
                // Expected.
            }
        } else {
            try {
                values.retainAll(null);
                // We have to tolerate a successful return (Sun bug 4802647)
                fail("Expected UnsupportedOperationException or NullPointerException.");
            } catch (UnsupportedOperationException | NullPointerException e) {
                // Expected.
            }
        }
        assertInvariants(map);
    }

    @Test
    public void testValuesClear() {
        final Map<String, Student> map;
        try {
            map = makePopulatedMap();
        } catch (UnsupportedOperationException e) {
            return;
        }

        Collection<Student> valueCollection = map.values();
        if (supportsClear) {
            valueCollection.clear();
            assertTrue(valueCollection.isEmpty());
        } else {
            try {
                valueCollection.clear();
                fail("Expected UnsupportedOperationException.");
            } catch (UnsupportedOperationException e) {
                // Expected.
            }
        }
        assertInvariants(map);
    }

    private static <String, Student> Entry<String, Student> mapEntry(String key, Student value) {
        return Collections.singletonMap(key, value).entrySet().iterator().next();
    }
}