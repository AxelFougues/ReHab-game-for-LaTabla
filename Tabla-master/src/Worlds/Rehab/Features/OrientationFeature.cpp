#include "../BlueprintWorld.h"

#ifdef isUsingOrientation
void BlueprintWorld::setOrientationVec(string name, vec2 value){
    if ( name=="Up" ){
        mUpVec = value;
    }
    setAreUserSettingsDirty();
}
map<string,vec2> BlueprintWorld::getOrientationVecs() const{
    map<string,vec2> m;
    m["Up"] = mUpVec;
    return m;
}
void BlueprintWorld::initSettings(){
    mUpVec = mDefaultUpVec;
}

XmlTree BlueprintWorld::getUserSettings() const{
    XmlTree xml("settings","");
    xml.push_back( XmlTree("UpVec", vecToString(mUpVec)) );
    return xml;
}

void BlueprintWorld::setUserSettings( XmlTree settingsXml ){
    if ( settingsXml.hasChild("settings") ){
        XmlTree xml = settingsXml.getChild("settings");
        getXml(xml, "UpVec", mUpVec );
    }
}
#endif
