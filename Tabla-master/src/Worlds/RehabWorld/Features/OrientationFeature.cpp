#include "../RehabWorld.h"

#ifdef isUsingOrientation
void RehabWorld::setOrientationVec(string name, vec2 value){
    if ( name=="Up" ){
        mUpVec = value;
    }
    setAreUserSettingsDirty();
}
map<string,vec2> RehabWorld::getOrientationVecs() const{
    map<string,vec2> m;
    m["Up"] = mUpVec;
    return m;
}
void RehabWorld::initSettings(){
    mUpVec = mDefaultUpVec;
}

XmlTree RehabWorld::getUserSettings() const{
    XmlTree xml("settings","");
    xml.push_back( XmlTree("UpVec", vecToString(mUpVec)) );
    return xml;
}

void RehabWorld::setUserSettings( XmlTree settingsXml ){
    if ( settingsXml.hasChild("settings") ){
        XmlTree xml = settingsXml.getChild("settings");
        getXml(xml, "UpVec", mUpVec );
    }
}
#endif
