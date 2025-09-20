
class Organization extends Obj {
	constructor(arg = false)
	{
		super("/api/organization", arg);
	}
    
    icon(){
        return this.data.icon;
    }

    title(){
        return this.data.name;
    }
    
    brief(){
        return this.data.brief;
    }
};

function toOrganizations(data) {
    var ret = [];
    for( const x of data)
        ret.push(new Organization(x));
    return ret;
}
