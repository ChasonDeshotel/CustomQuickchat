void
MemoryManager::MarkInvincible(class UObject* object) {
    if (object) {
        object->ObjectFlags &= ~EObjectFlags::RF_Transient;
        object->ObjectFlags &= ~EObjectFlags::RF_TagGarbage;
        object->ObjectFlags &= ~EObjectFlags::RF_PendingKill;
        object->ObjectFlags |= EObjectFlags::RF_DisregardForGC;
        object->ObjectFlags |= EObjectFlags::RF_RootSet;
    }
}

void
MemoryManager::MarkForDestroy(class UObject* object) {
    if (object) {
        object->ObjectFlags |= EObjectFlags::RF_Transient;
        object->ObjectFlags |= EObjectFlags::RF_TagGarbage;
        object->ObjectFlags |= EObjectFlags::RF_PendingKill;

        auto objectIt = std::find(m_createdObjects.begin(), m_createdObjects.end(), object);

        if (objectIt != m_createdObjects.end()) {
            m_createdObjects.erase(objectIt);
        }
    }
}
